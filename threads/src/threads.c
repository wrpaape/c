#include "threads.h"

/* queue interface */
extern inline struct Queue *init_queue(void);

extern inline void free_queue(struct Queue *queue);

extern inline void push_data(struct Queue *queue,
			     void *data);

extern inline void *pop_data(struct Queue *queue);


int main(int argc, char *argv[])
{
	return 0;
}


void *produce_strings(void *arg)
{
	return NULL;
}

void *scramble_strings(void *arg)
{
	return NULL;
}

void *unscramble_strings(void *arg)
{
	return NULL;
}

void *log_data(void *arg)
{
	return NULL;
}

void *free_data(void *arg)
{
	return NULL;
}

