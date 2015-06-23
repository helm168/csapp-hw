#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <unistd.h>
#include "socket_h.h"
#include "../hw10/rio.h"

#define HTTP_METHOD_GET "GET"

extern char **environ;

void doit(int connfd);
void client_error(int fd, char *cause, char *errnum, char *shortmsg,
	char *longmsg);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int size);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void get_mimetype(char *filename, char *mimetype);

int main(int argc, char **argv) {
	int port, clientlen;
	int listenfd, connfd;
	struct sockaddr_in clientaddr;
	int is_static;
	struct stat sbuf;
	
	if(argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(0);
	}	

	port = atoi(argv[1]);

	listenfd = open_listenfd(port);

	if(listenfd == -1) {
		fprintf(stderr, "cannot listen port %d\n", port);
		exit(0);
	}

	while(1) {
		clientlen = sizeof(clientaddr);
		connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
		doit(connfd);
		close(connfd);
	}

	exit(EXIT_SUCCESS);
}

void doit(int connfd) {
	rio_t rp;
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char filename[MAXLINE], cgiargs[MAXLINE];
	int is_static;
	struct stat sbuf;
	rio_readinitb(&rp, connfd);
	rio_readlineb(&rp, buf, MAXLINE); 
	sscanf(buf, "%s %s %s", method, uri, version);

	if(strcasecmp(method, HTTP_METHOD_GET)) {
		client_error(connfd, method, "501", "Not Implemented",
			"Tiny does not implement this method");
		return;
	}	
	read_requesthdrs(&rp);

	is_static = parse_uri(uri, filename, cgiargs);
	if(stat(filename, &sbuf) < 0) {
		client_error(connfd, filename, "404", "Not found",
			"Tiny couldn't find this file!");
		return;
	}

	if(is_static) {
		if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
			client_error(connfd, filename, "403", "Forbidden",
				"Tiny couldn't read the file!");
			return;
		}
		serve_static(connfd, filename, sbuf.st_size);
	} else {
		if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))  {
			client_error(connfd, filename, "403", "Forbidden",
				"Tiny couldn't run the cgi programe!");
			return;
		}
		serve_dynamic(connfd, filename, cgiargs);
	}
}

void client_error(int fd, char *cause, char *errnum, char *shortmsg,
	char *longmsg) {
	char buf[MAXLINE], body[MAXBUF];

	sprintf(body, "<html><title>Tiny Error.</title>");
	sprintf(body, "%s<body bgcolor=\"ffffff\">\r\n", body);
	sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
	sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
	sprintf(body, "%s<hr><em>The Tiny Web Server</em>\r\n", body);
	sprintf(body, "%s</body></html>", body);

	sprintf(buf, "HTTP/1.1 %s %s\r\n", errnum, shortmsg);
	rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-type: text/html\r\n");
	rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
	rio_writen(fd, buf, strlen(buf));
	rio_writen(fd, body, strlen(body)); 
}

void read_requesthdrs(rio_t *rp) {
	char buf[MAXLINE];

	rio_readlineb(rp, buf, MAXLINE);
	while(strcmp(buf, "\r\n")) {
		rio_readlineb(rp, buf, MAXLINE);
		printf("%s", buf);
	}
	return;
}

int parse_uri(char *uri, char *filename, char *cgiargs) {
	char *ptr;

	if(!strstr(uri, "cgi-bin")) {
		strcpy(cgiargs, "");
		strcpy(filename, ".");
		strcat(filename, uri);
		if(uri[strlen(uri)-1] == '/') {
			strcat(filename, "index.html");
		}
		return 1;
	} else {
		ptr = index(uri, '?');
		if(ptr) {
			strcpy(cgiargs, ptr+1);
			ptr = '\0';
		} else {
			strcpy(cgiargs, "");
		}	
		strcpy(filename, ".");
		strcat(filename, uri);
		return 0;
	}
}

void serve_static(int fd, char *filename, int filesize) {
	char mimetype[MAXLINE];
	int srcfd;
	char *srcp;
	char buf[MAXLINE];

	get_mimetype(filename, mimetype);
	sprintf(buf, "HTTP1.1 200 OK\r\n");
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, mimetype);
	rio_writen(fd, buf, strlen(buf));

	srcfd = open(filename, O_RDONLY, 0);
	srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
	close(srcfd);
	rio_writen(fd, srcp, filesize);
	munmap(srcp, filesize);
}

void get_mimetype(char *filename, char *mimetype) {
	if(strstr(filename, ".html")) {
		strcpy(mimetype, "text/html");
	} else if(strstr(filename, ".gif")) {
		strcpy(mimetype, "image/gif");
	} else if(strstr(filename, ".jpg")) {
		strcpy(mimetype, "image/jpg");
	} else {
		strcpy(mimetype, "text/plain");
	}
}

void serve_dynamic(int fd, char *filename, char *cgiargs) {
	char buf[MAXLINE], *emptylist[] = {NULL};

	sprintf(buf, "HTTP/1.1 200 OK\r\n");
	rio_writen(fd, buf, strlen(buf));
	sprintf(buf, "Server: Tiny Web Server\r\n");
	rio_writen(fd, buf, strlen(buf));

	if(fork() == 0) {
		setenv("QUERY_STRING", cgiargs, 1);
		dup2(fd, STDOUT_FILENO);
		execve(filename, emptylist, environ);
	}
	wait(NULL);
}
