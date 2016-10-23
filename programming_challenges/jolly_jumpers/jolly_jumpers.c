#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol */
#include <stdio.h>	/* getline */

#define READ_INTEGER(STRING)						\
(int) strtol(STRING,							\
	     &STRING,							\
	     10)

static inline bool
is_jolly_jumper(char *sequence)
{
	bool *restrict diff_ptr;
	unsigned int count;
	int integer;
	int next;
	unsigned int diff;

	count = (unsigned int) READ_INTEGER(sequence);

	bool *const restrict diff_set = calloc(count,
					       sizeof(bool));

	if (diff_set == NULL)
		exit(1);

	diff_set[0] = true;

	integer = READ_INTEGER(sequence);

	while (*sequence != '\n') {
		next = READ_INTEGER(sequence);

		diff = (next > integer)
		     ? (next - integer)
		     : (integer - next);

		if (diff >= count) {
			free(diff_set);
			return false;
		}

		diff_ptr = &diff_set[diff];

		if (*diff_ptr) {
			free(diff_set);
			return false;
		}

		*diff_ptr = true;

		integer = next;
	}

	free(diff_set);
	return true;
}

int
main(void)
{
	char *line;
	size_t capacity;

	line	 = NULL;
	capacity = 0;

	while (getline(&line,
		       &capacity,
		       stdin) > 0)
		puts(is_jolly_jumper(line) ? "Jolly" : "Not jolly");

	free(line);
	return 0;
}
