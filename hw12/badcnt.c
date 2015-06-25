#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int cnt = 0;

void *thread(void *vargp);

int main(int argc, char **argv) {
	pthread_t tid1, tid2;
	int niters;

	if(argc != 2) {
		printf("usage: %s <niters>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	niters = atoi(argv[1]);

	pthread_create(&tid1, NULL, thread, &niters);
	pthread_create(&tid2, NULL, thread, &niters);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	if(cnt != 2 * niters) {
		printf("BOOM! cnt=%d\n", cnt);
	} else {
		printf("OK! cnt=%d\n", cnt);
	}

	exit(EXIT_SUCCESS);
}

void *thread(void *vargp) {
	int i, niters = *((int *)vargp);

	for(i = 0; i < niters; i++) {
		cnt++;
	}

	return NULL;
}
