#include <unistd.h>	/* write */

struct Interval {
	const char *restrict from;
	const char *restrict until;
};


static inline char *
put_interval(char *restrict buffer,
	     const struct Interval *const restrict interval)
{
	const char *restrict from;

	from = interval->from;
	const char *const restrict until = interval->until;

	while (from < until) {
		*buffer = *from;
		++buffer;
		++from;
	}

	return buffer;
}





void
longest_common_subsquence(struct Interval *const restrict interval,
			  const char *restrict sequence1,
			  const char *restrict sequence2)
{
}



int
main(void)
{
	struct Interval interval;
	static char buffer[128];
	char *restrict ptr;

	longest_common_subsquence(&interval,
				  "ABAB",
				  "BABA");

	ptr = put_interval(&buffer[0],
			   &interval);

	*ptr = '\n';
	++ptr;

	const int success = (write(STDOUT_FILENO,
				   &buffer[0],
				   ptr - &buffer[0]) >= 0);

	if (!success)
		(void) write(STDERR_FILENO,
			     "write failure\n",
			     sizeof("write failure\n"));

	return success;
}
