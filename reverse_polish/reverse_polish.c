#include <unistd.h>	/* read/write */
#include <stdlib.h>	/* exit */
#include <stdio.h>	/* printf */
#include <stdbool.h>	/* bool */


#define BUFFER_SIZE 1024

#define ASCII_TO_DIGIT(TOKEN) (((int) (TOKEN)) & 15u)








static inline void
read_expression(char *const restrict buffer,
		const size_t size_max)
{
	const char *restrict message;
	FILE *restrict stream;
	int exit_status;

	const ssize_t size_read = read(STDIN_FILENO,
				       buffer,
				       size_max);


	if (size_read > 0) {
		if (size_read < size_max) {
			buffer[size_read - 1] = '\0';	/* remove newline */
			return;
		} else {
			message	    = "read overflow\n";
			stream	    = stderr;
			exit_status = 1;
		}
	} else {
		if (size_read < 0) {
			message	    = "read failure\n";
			stream	    = stderr;
			exit_status = 1;
		} else {
			message	    = "done\n";
			stream	    = stdout;
			exit_status = 0;
		}
	}

	fputs(message,
	      stream);
	exit(exit_status);
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
	struct Node *restrict node_alloc_ptr;
	struct Node *restrict head;
	int *restrict head_value;
	int node_value;

	char token;

	node_alloc_ptr = &node_alloc[0];

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

		case '0' ... '9':
			node = node_alloc_ptr;
			++node_alloc_ptr;

			node->value = ASCII_TO_DIGIT(token);
			node->next  = head;
			head = node;
			break;

		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			if (head == NULL) {
				*failure = "imbalanced expression\n";
				return false;
			}

			node = head->next;
			if (node == NULL) {
				*failure = "imbalanced expression\n";
				return false;
			}

			head_value = &head->value;
			node_value = node->value;

			switch (token) {
			case '+':
				*head_value += node_value;
				break;

			case '-':
				*head_value  = node_value - (*head_value);
				break;

			case '*':
				*head_value *= node_value;
				break;

			case '/':
				*head_value  = node_value / (*head_value);
				break;

			default:
				*head_value = node_value % (*head_value);
			}

			head->next = node->next;
			break;

		default:
			*failure = "invalid operation\n";
			return false;
		}

		++buffer;
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
