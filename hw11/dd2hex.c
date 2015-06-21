#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADDR_SEG_NUM 4
#define ADDR_DELIM "."

int main(int argc, char **argv) {
	if(argc != 2) return;

	unsigned int addr;
	char *addrptr;
	
	char *addrseg[ADDR_SEG_NUM];
	int parsenum;

	char *saveptr;
	char *str = argv[1];
	char *token;
	for(parsenum = 0; ; parsenum++, str = NULL) {
		token = strtok_r(str, ADDR_DELIM, &saveptr);
		if(token == NULL) {
			break;
		} else {
			addrseg[parsenum] = token;
		}
	}

	if(parsenum != ADDR_SEG_NUM) {
		printf("error id address.\n");
		exit(-1);
	}

	addrptr = (char *)(&addr);	
	parsenum--;
	while(parsenum >= 0) {
		*addrptr = atoi(addrseg[parsenum]);
		addrptr++;
		parsenum--;
	}

	printf("0x%x\n", addr);
	exit(EXIT_SUCCESS);
}
