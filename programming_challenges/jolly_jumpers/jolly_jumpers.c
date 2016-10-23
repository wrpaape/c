#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol */

/* '0'..'9' → 0..9 */
#define ASCII_TO_DIGIT(ASCII) ((ASCII) & 15u)

#define WRITE_OUTPUT_LINE(OUTPUT)					\
(void) write(STDOUT_FILENO,						\
	     OUTPUT "\n",						\
	     sizeof(OUTPUT))

static inline bool
string_to_integer(int *const restrict integer,
		  const char *restrict string)
__attribute__((always_inline));

static inline bool
string_to_integer(int *const restrict integer,
		  const char *restrict string)
{
	strtol
	/* int val0; */
	/* int val1; */
	/* int val2; */
	/* int val3; */
	/* int token; */

	/* token = (int) *string; */

	/* if (token == '-') { */
	/* 	++string; */
	/* 	token = (int) *string; */
	/* 	val0  = -1; */
	/* } else { */
	/* 	val0 = 1; */
	/* } */

	/* val0 *= ASCII_TO_DIGIT(token); */

	/* ++string; */
	/* token = (int) *string; */

	/* if (token == 0) { */
	/* 	*integer = val0; */
	/* 	return true; */
	/* } */

	/* val1 = ASCII_TO_DIGIT(token); */

	/* ++string; */
	/* token = (int) *string; */

	/* if (token == 0) { */
	/* 	*integer = (val0 * 10) + val1; */
	/* 	return true; */
	/* } */

	/* val2 = ASCII_TO_DIGIT(token); */

	/* ++string; */
	/* token = (int) *string; */

	/* if (token == 0) { */
	/* 	*integer = (val0 * 100) + (val1 * 10) + val2; */
	/* 	return true; */
	/* } */

	/* const bool success = ((val0 < 3) && (string[1] == '\0')); */

	/* if (success) { */
	/* 	val3 = ASCII_TO_DIGIT(token); */
	/* 	*integer = (val0 * 1000) + (val1 * 100) + (val2 * 10) + val3; */
	/* } */

	/* return success; */
}

int
main(int argc,
     char **argv)
{
	bool *restrict diff_ptr;
	char *restrict *restrict from;
	int integer;
	int next;
	unsigned int diff;

	static bool diff_set[3000] = {
		[0] = true
	};


	from = argv + 2;

	const unsigned int count = argc - 2;

	char *restrict *const restrict until = from + count;

	if (!string_to_integer(&integer,
			       *from)) {
		WRITE_OUTPUT_LINE("Not jolly");
		return 0;
	}


	while (1) {
		++from;

		if (from == until) {
			WRITE_OUTPUT_LINE("Jolly");
			return 0;
		}

		if (!string_to_integer(&next,
				       *from)) {
			WRITE_OUTPUT_LINE("Not jolly");
			return 0;
		}

		diff = (next > integer)
		     ? (next - integer)
		     : (integer - next);

		if (diff >= count) {
			WRITE_OUTPUT_LINE("Not jolly");
			return 0;
		}

		diff_ptr = &diff_set[diff];

		if (*diff_ptr) {
			WRITE_OUTPUT_LINE("Not jolly");
			return 0;
		}

		*diff_ptr = true;

		integer = next;
	}
}
