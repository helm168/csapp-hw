#include "../hw10/rio.h"
#include "socket_h.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int clientfd, port;
	char *host, buf[MAXLINE];
	rio_t rio;

	if(argc != 3) {
		fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
		exit(0);
	}
	host = argv[1];
	port = atoi(argv[2]);

	clientfd = open_clientfd(host, port);
	if(clientfd == -1) {
		printf("socket error.\n");
		exit(0);
	} else if(clientfd == -2){
		printf("network error.\n");
		exit(0);
	}
	rio_readinitb(&rio, clientfd);

	while(fgets(buf, MAXLINE, stdin) != NULL) {
		rio_writen(clientfd, buf, strlen(buf));
		rio_readlineb(&rio, buf, MAXLINE);
		fputs(buf, stdout);
	}

	close(clientfd);
	exit(EXIT_SUCCESS);
}
