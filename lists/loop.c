#include <stdlib.h>	/* malloc, free */
#include <stdio.h>	/* puts */
#include <assert.h>	/* assert */

struct Node {
	unsigned int value;
	struct Node *next;
};

static inline void
init_ascending_list(struct Node *restrict node,
		    const unsigned int count_nodes)
{
	unsigned int value;
	struct Node *restrict next;

	if (count_nodes == 0)
		return;

	value = 0;

	while (1) {
		node->value = value;
		++value;

		if (value == count_nodes) {
			node->next = NULL;
			return;
		}

		next = node + 1;
		node->next = next;
		node = next;
	}

}

static inline size_t
find_length_loop(struct Node *const restrict start)
{
	struct Node *restrict node;
	size_t length_loop;

	node	    = start;
	length_loop = 1;

	while (1) {
		node = node->next;

		if (node == start)
			return length_loop;

		++length_loop;
	}
}


static inline struct Node *
find_loop(struct Node *restrict head)
{
	struct Node *restrict slow;
	struct Node *restrict fast;
	size_t rem_steps;

	slow = head;
	fast = head;

	while (1) {
		if (fast == NULL)
			return NULL;

		slow = slow->next;
		fast = fast->next;

		if (fast == NULL)
			return NULL;

		fast = fast->next;

		if (fast == slow)
			break;
	}

	rem_steps = find_length_loop(fast);

	fast = head;
	while (1) {
		fast = fast->next;

		if (rem_steps == 1)
			break;

		--rem_steps;
	}

	slow = head;

	while (fast != slow) {
		slow = slow->next;
		fast = fast->next;
	}

	return fast;
}

int
main(void)
{
	struct Node *restrict node;
	struct Node nodes[10];
	struct Node *const restrict head = &nodes[0];

	init_ascending_list(head,
			    10);

	assert(find_loop(head) == NULL);

	nodes[9].next = &nodes[3];

	node = find_loop(head);

	if (node == NULL)
		puts("error loop not detected");
	else
		printf("loop: %u\n", node->value);

	return 0;
}
