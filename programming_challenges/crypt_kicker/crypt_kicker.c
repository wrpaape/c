#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol */
#include <stdio.h>	/* getline */

#define READ_INTEGER(STRING)						\
(int) strtol(STRING,							\
	     &STRING,							\
	     10)



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
