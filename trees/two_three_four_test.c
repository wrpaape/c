#include "two_three_four_test.h"

int
main(void)
{
	return 0;
}


/* tree compare function */
int
compare_strings(void *key1,
		void *key2)
{
	unsigned char token1;
	unsigned char token2;
	const unsigned char *restrict string1 = (const unsigned char *) key1;
	const unsigned char *restrict string2 = (const unsigned char *) key2;

	while (1) {
		token1 = *string1;
		token2 = *string2;

		if (token1 != token2)
			return token1 - token2;

		if (token1 == '\0')
			return 0;

		++string1;
		++string2;
	}
}

/* tree stringify function */
char *
put_string(char *restrict buffer,
	   void *pointer)
{
	const char *restrict string = (const char *) pointer;

	while (*string != '\0') {
		*buffer = *string;
		++buffer;
		++string;
	}

	return buffer;
}


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline char *
get_input(void)
{
	static char buffer[4096];
	static char *restrict next	 = &buffer[0];
	static const char *restrict last = &buffer[4095];
	char *restrict input;
	ssize_t size_read;

	READ_INPUT_SIZE(size_read,
			next,
			last - next);

	input = next;

	next += size_read;

	if (next == last) {
		WRITE_OUTPUT_LITERAL("OUT OF SPACE\n");
		exit(0);
	}

	return input;
}
