#ifndef SOCKET_H_H
#define SOCKET_H_H

#define MAXLINE 8192
#define MAXBUF 8192
#define LISTENQ 1024

typedef struct sockaddr SA;

int open_clientfd(char *hostname, int port);
int open_listenfd(int port);

#endif 
