#include <parallel/pthread.h>	/* pthread API, error handlers */
#include <parallel/semaphore.h>	/* semaphore_wait */
#include "threads.h"

int main(int argc, char *argv[])
{
	return 0;
}


void *produce_strings(void *arg)
{
	return NULL;
}

void *process_strings(void *arg)
{
	return NULL;
}

/* simple queue operations */
void push_data(struct Queue *queue,
	       void *data)
{
	struct QueueNode *node;

	HANDLE_MALLOC(node, sizeof(struct QueueNode));

	node->data = data;
	node->prev = NULL;

	if (queue->last == NULL) {
		queue->next = node;
	} else {
		*(queue->last) = node;
	}


	queue->last = &(node->prev);
}

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
