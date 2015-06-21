#include "socket_h.h"
#include "../hw10/rio.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

void echo(int connfd);

int main(int argc, char **argv) {
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;
	if(argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);

	listenfd = open_listenfd(port);
	if(listenfd == -1) {
		printf("cannot listen on port:%d\n", port);
		exit(0);
	}
	while(1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);

		hp = gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,
			sizeof(clientaddr.sin_addr.s_addr), AF_INET);
		haddrp = inet_ntoa(clientaddr.sin_addr);
		printf("server connected to %s (%s)\n", hp->h_name, haddrp);

		echo(connfd);
		close(connfd);
	}
	exit(EXIT_SUCCESS);
}
