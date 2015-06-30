#ifndef SBUF_H
#define SBUF_H

typedef struct {
	int *buf; /* Buffer array */
	int n; /* Maximum number of slots */
	int front; /* buf[(front+1)%n] is first item */
	int rear; /* buf[rear%n] is last item */
	sem_t mutex; /* Protects accesses to buf */
	sem_t slots; /* Counts available slots */
	set_t items; /* Counts available items */
} sbuf_t;

#endif
