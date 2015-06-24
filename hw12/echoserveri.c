#include "../hw11/socket_h.h"
#include "../hw10/rio.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

void echo(int connfd);
void command();

int main(int argc, char **argv) {
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char *haddrp;
	fd_set read_set, ready_set;

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

	FD_ZERO(&read_set);
	FD_SET(STDIN_FILENO, &read_set);
	FD_SET(listenfd, &read_set);

	clientlen = sizeof(clientaddr);

	while(1) {
		ready_set = read_set;
		select(listenfd+1, &ready_set, NULL, NULL, NULL);
		if(FD_ISSET(STDIN_FILENO, &ready_set))
			command();
		if(FD_ISSET(listenfd, &ready_set)) {
			connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
			hp = gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,
				sizeof(clientaddr.sin_addr.s_addr), AF_INET);

			echo(connfd);
			close(connfd);
		}
	}
	exit(EXIT_SUCCESS);
}

void command() {
	char buf[MAXLINE];
	if(!fgets(buf, MAXLINE, stdin))
		exit(0);
	printf("%s", buf);
}
