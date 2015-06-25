#include <pthread.h>
#include "../hw10/rio.h"
#include "../hw11/socket_h.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>

void echo(int fd);
void *thread(void *vargp);

int main(int argc, char **argv) {
	int listenfd, *connfdp;
	struct sockaddr_in clientaddr;
	socklen_t clientlen;
	int port;
	pthread_t tid;

	if(argc != 2) {
		fprintf(stderr, "Usage: %s <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	port = atoi(argv[1]);

	listenfd = open_listenfd(port);
	if(listenfd == -1) {
		fprintf(stderr, "can not open on port %d\n", port);
		exit(EXIT_FAILURE);
	}

	clientlen = sizeof(clientaddr);
	while(1) {
		connfdp = malloc(sizeof(int));
		*connfdp = accept(listenfd, (SA *)&clientaddr, &clientlen);
		pthread_create(&tid, NULL, thread, connfdp);
	}
}


void *thread(void *vargp) {
	int connfd = *((int *)vargp);
	free(vargp);
	pthread_detach(pthread_self());
	echo(connfd);
	close(connfd);
	return NULL;
}
