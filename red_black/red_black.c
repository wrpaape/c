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

		case 1:
			switch (input[0]) {
			case 'b':
				return true;

			case 'q':
				return false;

			default:
				break;
			}

		default:
			break;
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

static inline bool
find_mode(void)
{
	return true;
}

int
main(void)
{
	unsigned char input[128];

	while (1) {
		GET_CHAR(MAIN_PROMPT,
			 input);

		switch (input[0]) {
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
