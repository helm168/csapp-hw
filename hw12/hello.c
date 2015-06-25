#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread(void *vargp);

int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	pthread_join(tid, NULL);
	exit(EXIT_SUCCESS);
}

void *thread(void *vargp) {
	printf("Hello, world!\n");
	return NULL;
}
