#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define N 5

void *thread(void *vargp);

sem_t mutex;

int main() {
	pthread_t tid[N];
	int i;

	sem_init(&mutex, 0, 1);

	i = -1;
	do {
		sem_wait(&mutex);
		i++;
		pthread_create(&tid[i], NULL, thread, &i);
	} while(i < N);

	for(i = 0; i < N; i++) {
		pthread_join(tid[i], NULL);
	}
	exit(EXIT_FAILURE);
}

void *thread(void *vargp) {
	int myid = *((int *)vargp);
	printf("Hello from thread %d\n", myid);
	sem_post(&mutex);
	return NULL;
}
