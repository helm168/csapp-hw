#include "../hw11/socket_h.h"
#include "../hw10/rio.h"
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

typedef struct {
	int maxfd;
	fd_set read_set;
	fd_set ready_set;
	int nready;
	int maxi;
	int clientfd[FD_SETSIZE];
	rio_t clientrio[FD_SETSIZE];
} pool;

int byte_cnt = 0;

void initpool(int fd, pool *p);
void addclient(int fd, pool *p);
void checkclients(pool *p);

int main(int argc, char **argv) {
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in clientaddr;
	static pool p;

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

	clientlen = sizeof(clientaddr);

	initpool(listenfd, &p);

	while(1) {
		p.ready_set = p.read_set;
		p.nready = select(p.maxfd+1, &p.ready_set, NULL, NULL, NULL);
		if(FD_ISSET(listenfd, &p.ready_set)) {
			connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
			addclient(connfd, &p);
		}
		checkclients(&p);
	}
	exit(EXIT_SUCCESS);
}

void initpool(int fd, pool *p) {
	int i;
	p->maxi = -1;
	for(i = 0; i < FD_SETSIZE; i++) {
		p->clientfd[i] = -1;
	}	

	p->maxfd = fd;
	FD_ZERO(&p->read_set);
	FD_SET(fd, &p->read_set);
}

void addclient(int fd, pool *p) {
	int i;
	p->nready--;

	for(i = 0; i < FD_SETSIZE; i++) {
		if(p->clientfd[i] < 0) {
			p->clientfd[i] = fd;
			rio_readinitb(&p->clientrio[i], fd); 
			
			FD_SET(fd, &p->read_set);

			if(fd > p->maxfd) {
				p->maxfd = fd;
			}

			if(i > p->maxi) {
				p->maxi = i;
			}
			break;
		}
	}
	if(i == FD_SETSIZE) {
		fprintf(stderr, "add client error: Too many clients");
		exit(EXIT_FAILURE);
	}
}

void checkclients(pool *p) {
	int i, fd, n;
	rio_t rio;
	char buf[MAXLINE];

	for(i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
		fd = p->clientfd[i];
		rio = p->clientrio[i];

		if(fd > 0 && FD_ISSET(fd, &p->ready_set)) {
			p->nready--;
			if((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
				byte_cnt += n;
				printf("Server received %d (%d total) bytes on fd %d\n",
					n, byte_cnt, fd);
				rio_writen(fd, buf, n);
			} else {
				close(fd);
				FD_CLR(fd, &p->read_set);
				p->clientfd[i] = -1;
			}
		}
	}
}
