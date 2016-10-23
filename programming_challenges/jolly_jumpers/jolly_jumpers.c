#include <stdlib.h>	/* malloc, free, perror */
#include <stdio.h>	/* puts */
#include <errno.h>	/* errno */
#include <limits.h>	/* INT_MAX */

int
string_to_integer(int *const restrict value_ptr,
		  const char *restrict string)
{
	const long value = strtol(string,
				  NULL,
				  10);

	const int status = ((value == 0) && (errno != 0))
			|| (value > INT_MAX);

	if (!status)
		*value_ptr = (int) value;

	return status;
}

int
args_to_integers(const int *restrict *const restrict integers_ptr,
		 char **args,
		 const unsigned int count)
{
	int status;
	int *restrict integer;

	integer = malloc(sizeof(int) * count);

	status = (integer == NULL);

	if (!status) {
		*integers_ptr = integer;

		const int *const restrict until = integer + count;

		while (1) {
			status = string_to_integer(integer,
						   *args);

			if (status)
				break;

			++integer;

			if (integer == until)
				break;

			++args;
		}
	}

	return status;
}

int
is_jolly_jumper(const int *restrict integer,
		const unsigned int count)
{
	unsigned int abs_diff;
	const int *restrict next;

	const int *const restrict until = integer + count;

	while (1) {
		next = integer + 1;

		if (next == until)
			return 1;

		abs_diff = (unsigned int) abs(*next - *integer);

		if ((abs_diff == 0) || (abs_diff >= count))
			return 0;

		integer = next;
	}
}


int
main(int argc,
     char *argv[])
{
	const int *restrict integers;

	if (argc < 4) {
		puts("Jolly");
		return 0;
	}

	const unsigned int count = argc - 2u;

	const int exit_status = args_to_integers(&integers,
						 &argv[2],
						 count);

	if (exit_status) {
		perror("args_to_integers failure");

	} else {
		puts(is_jolly_jumper(integers,
				     count) ? "Jolly" : "Not jolly");

		free((void *) integers);
	}


	return exit_status;
}
