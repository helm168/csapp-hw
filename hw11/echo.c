#include <stdio.h>
#include "../hw10/rio.h"
#include "socket_h.h"
#include <stdlib.h>

void echo(int connfd) {
	size_t n;
	char buf[MAXLINE];
	rio_t rio;

	rio_readinitb(&rio, connfd);
	while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
		printf("server receiverd %zu bytes\n",n);
		rio_writen(connfd, buf, n);
	}
}
