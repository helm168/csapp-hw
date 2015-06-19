#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc != 2) return;
	struct stat fstat;
	char *type;
	char *readok;
	stat(argv[1], &fstat);
	if(S_ISREG(fstat.st_mode)) {
		type = "regular";
	} else if(S_ISDIR(fstat.st_mode)) {
		type = "directory";
	} else {
		type = "other";
	}
	if((fstat.st_mode & S_IRUSR)) /* Check read access */
		readok = "yes";
	else
		readok = "no";
	
	printf("type: %s, read: %s\n", type, readok);
	return 0;
}	
	
