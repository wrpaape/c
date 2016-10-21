#include <stdio.h>

struct Node {
	char value;
	struct Node *next;
};

static inline void
init_list(struct Node *restrict node,
	  const size_t count)
{
	struct Node *restrict next;
	char value;

	if (count == 0lu)
		return;

	const struct Node *const restrict until = node + count;

	value = 'a';

	while (1) {
		node->value = value;

		next = node + 1l;

		if (next == until) {
			node->next = NULL;
			return;
		}

		node->next = next;

		node = next;
		++value;
	}
}

struct Node *
do_reverse(struct Node *const restrict prev,
	   struct Node *const restrict node)
{
	struct Node *const restrict next = node->next;

	node->next = prev;

	return (next == NULL)
	     ? node
	     : do_reverse(node,
			  next);
}

static inline void
reverse_nodes(struct Node *restrict *const restrict head)
{
	struct Node *const restrict node = *head;

	if (node != NULL)
		*head = do_reverse(NULL,
				   node);
}

static inline void
print_list(struct Node *restrict node)
{

	if (node == NULL) {
		puts("{}");
		return;
	}

	printf("{\n\t%c", node->value);

	for (node = node->next; node != NULL; node = node->next)
		printf(", %c", node->value);

	puts("\n}");
}

int
main(void)
{
	struct Node nodes[26];
	struct Node *restrict head = &nodes[0];

	init_list(head,
		  26);

	print_list(head);

	reverse_nodes(&head);

	print_list(head);

	return 0;
}
