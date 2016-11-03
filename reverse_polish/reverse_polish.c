#include <unistd.h>	/* read/write */
#include <stdlib.h>	/* exit */
#include <stdio.h>	/* printf */
#include <stdbool.h>	/* bool */


#define BUFFER_SIZE 1024

#define ASCII_TO_DIGIT(ASCII) (((unsigned int) (ASCII)) & 15u)








static inline void
read_expression(char *const restrict buffer,
		const size_t size_max)
{
	const char *restrict failure;

	const ssize_t size_read = read(STDIN_FILENO,
				       buffer,
				       size_max);

	if (size_read < 0) {
		failure = "read failure\n";
	} else if (size_read == size_max) {
		failure = "read overflow\n";
	} else {
		buffer[size_read - 1] = '\0';	/* remove newline */
		return;
	}

	fputs(failure,
	      stderr);
	exit(1);
	__builtin_unreachable();
}

static inline bool
evaluate_expression(int *const restrict result,
		    const char *restrict buffer,
		    const char *restrict *const restrict failure)
{
	struct Node {
		int value;
		struct Node *next;
	};

	static struct Node node_alloc[BUFFER_SIZE / 2];

	struct Node *restrict node;
	struct Node *restrict head;

	char token;

	node = &node_alloc[0];
	head = NULL;

	while (1) {

		token = *buffer;

		switch (token) {
		case '\0':
			if ((head == NULL) || (head->next != NULL)) {
				*failure = "imbalanced expression\n";
				return false;
			}

			*result = head->value;
			return true;

		default:
			*failure = "invalid operation\n";
			return false;
		}
	}
}



int
main(void)
{
	static char buffer[BUFFER_SIZE];
	const char *restrict failure;
	int result;

	while (1) {
		read_expression(&buffer[0],
				sizeof(buffer));

		if (evaluate_expression(&result,
					&buffer[0],
					&failure))
			printf("%d\n",
			       result);
		else
			fputs(failure,
			      stderr);
	}

	return 0;
}
