#include "red_black.h"

static struct RedBlackNode *restrict tree;

static inline bool
insert_mode(void)
{
	unsigned char input[128];
	struct Key *restrict key;
	ssize_t size_read;

	while (1) {
		WRITE_LITERAL(INSERT_PROMPT);
		size_read = READ_INPUT(input);

		switch (size_read) {
		case -1:
			EXIT_ON_FAILURE("read failure");

		case 0:
			WRITE_LITERAL(INVALID_INPUT);
			continue;

		case 2:
			if (input[1] != '\n')
				break;

			/* fall through */

		case 1:
			switch (input[0]) {
			case 'b':
				return true;

			case 'q':
				return false;

			case 'p':
				red_black_print(tree);
				continue;

			default: /* fall through */;
			}

		default:
			--size_read; /* trim '\n' */
		}

		key = pop_key();

		make_key(key,
			 &input[0],
			 (size_t) size_read);

		red_black_insert(&tree,
				 key);
	}
}

static inline bool
delete_mode(void)
{
	return true;
}

static inline void
do_find_mode(const struct Key *const restrict key)
{
	char buffer[256];
	char *restrict ptr;

	ptr = &buffer[0];

	*ptr = '\n'; ++ptr;

	if (red_black_find(tree,
			   key)) {
		*ptr = 'f'; ++ptr;
		*ptr = 'o'; ++ptr;
		*ptr = 'u'; ++ptr;
		*ptr = 'n'; ++ptr;
		*ptr = 'd'; ++ptr;

	} else {
		*ptr = 'd'; ++ptr;
		*ptr = 'i'; ++ptr;
		*ptr = 'd'; ++ptr;
		*ptr = ' '; ++ptr;
		*ptr = 'n'; ++ptr;
		*ptr = 'o'; ++ptr;
		*ptr = 't'; ++ptr;
		*ptr = ' '; ++ptr;
		*ptr = 'f'; ++ptr;
		*ptr = 'i'; ++ptr;
		*ptr = 'n'; ++ptr;
		*ptr = 'd'; ++ptr;
	}

	*ptr = ' '; ++ptr;
	*ptr = 'k'; ++ptr;
	*ptr = 'e'; ++ptr;
	*ptr = 'y'; ++ptr;
	*ptr = ':'; ++ptr;
	*ptr = ' '; ++ptr;

	ptr = put_key(ptr,
		      key);

	*ptr = '\n'; ++ptr;
	*ptr = '\n'; ++ptr;

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}

static inline bool
find_mode(void)
{
	unsigned char input[128];
	struct Key key;
	ssize_t size_read;

	while (1) {
		WRITE_LITERAL(FIND_PROMPT);
		size_read = READ_INPUT(input);

		switch (size_read) {
		case -1:
			EXIT_ON_FAILURE("read failure");

		case 0:
			WRITE_LITERAL(INVALID_INPUT);
			continue;

		case 2:
			if (input[1] != '\n')
				break;

			/* fall through */

		case 1:
			switch (input[0]) {
			case 'b':
				return true;

			case 'q':
				return false;

			case 'p':
				red_black_print(tree);
				continue;

			default: /* fall through */;
			}

		default:
			--size_read; /* trim '\n' */
		}

		input[size_read] = '\0';

		key_init(&key,
			 &input[0],
			 (size_t) size_read);

		do_find_mode(&key);
	}
}

int
main(void)
{
	unsigned char input[128];

	while (1) {
		GET_CHAR(MAIN_PROMPT,
			 input);

		switch (input[0]) {
		case 'p':
			red_black_print(tree);
			break;

		case 'd':
			if (delete_mode())
				break;

			return 0;

		case 'f':
			if (find_mode())
				break;

			return 0;

		case 'i':
			if (insert_mode())
				break;

		case 'q':
			return 0;

		default:
			WRITE_LITERAL(INVALID_INPUT);
		}
	}
}
