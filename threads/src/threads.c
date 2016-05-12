#include <parallel/pthread.h>	/* pthread API, error handlers */
#include <parallel/semaphore.h>	/* semaphore_wait */
#include "threads.h"

int main(int argc, char *argv[])
{
	return 0;
}


void *produce_strings(void *arg)
{
}

void *process_strings(void *arg)
{
}

/* simple queue operations */
void *pop_data(struct Queue *queue)
{
	struct QueueNode *node = queue->next;

	if (node == NULL)
		return NULL;

	void *data = node->data;

	queue->next = node->prev;

	free(node);

	return data;
}

void *pop_data(struct Queue *queue)
