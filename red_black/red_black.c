#include "red_black.h"


static struct RedBlackNode *restrict tree;

int
main(void)
{
	char input[128];

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

static inline bool
insert_mode(void)
{
}
