#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	if(argc != 2) return;
	int ipnum = (int)strtol(argv[1], NULL, 16);
	char *b = (char *)&ipnum;
	int iplen = sizeof(ipnum);
	int i;
	char iphum[16];
	char dotbuf[2];
	char numbuf[4];
	iphum[0] = 0; 
	//not good here, write dead for little endian 
	for(i = iplen-1; i >= 0; i--) {
		sprintf(numbuf, "%u", *(b + i) & 0xff);
		strcat(iphum, numbuf);
		if(i != 0) {
			sprintf(dotbuf, "%c", '.');
			strcat(iphum, dotbuf);
		}	
	}
	printf("%s\n", iphum);
}
