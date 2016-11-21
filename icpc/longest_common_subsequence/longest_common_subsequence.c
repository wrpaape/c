#include <unistd.h>	/* write */
#include <stdlib.h>	/* free, malloc */
#include <stdio.h>	/* perror */

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

unsigned int
string_length(const char *const restrict string)
{
	register const char *restrict ptr;

	for (ptr = string; *ptr != '\0'; ++ptr);

	return ptr - string;
}



unsigned int **
create_table(const unsigned int size1,
	     const unsigned int size2)
{
	unsigned int *restrict *restrict row;
	unsigned int *restrict cols;

	unsigned int *restrict *const restrict table
	= malloc(  (sizeof(unsigned int *) * size1)
		 + (sizeof(unsigned int)   * (size1 * size2)));


	if (table == NULL) {
		perror("malloc");
		exit(1);
	}

	row = table;

	unsigned int *const restrict *const restrict row_until
	= row + size1;

	cols = (unsigned int *restrict) row_until;

	while (1) {
		*row = cols;
		++row;

		if (row == row_until)
			return (unsigned int **) table;

		cols += size2;
	}
}



static inline void
do_longest_common_subsquence(struct Interval *const restrict interval,
			     const char *const restrict sequence1,
			     const unsigned int length1,
			     const char *const restrict sequence2,
			     const unsigned int length2,
			     unsigned int *const restrict *const restrict table)
{
	unsigned int i1;
	unsigned int i2;
	unsigned int i1_prev;
	unsigned int i2_prev;
	unsigned int max_common;
	unsigned int common;

	i2 = 0;
	do {
		table[0][i2] = 0;
		++i2;
	} while (i2 < length2);

	max_common = 0;

	i1_prev = 0;
	i1      = 1;
	while (1) {
		table[i1][0] = 0;

		i2_prev = 0;
		i2      = 1;

		while (1) {
			if (sequence1[i1] == sequence2[i2]) {
				common = table[i1_prev][i2_prev] + 1;

				if (common > max_common) {
					max_common = common;
					interval->until = &sequence2[i2 + 1];
					interval->from  = interval->until - common;
				}

				table[i1][i2] = common;

			} else {
				table[i1][i2] = 0;
			}

			if (i2 == length2)
				break;

			i2_prev = i2;
			++i2;
		}

		if (i1 == length1)
			return;

		i1_prev = i1;
		++i1;
	}
}

static inline void
longest_common_subsquence(struct Interval *const restrict interval,
			  const char *const restrict sequence1,
			  const char *const restrict sequence2)
{
	const unsigned int length1 = string_length(sequence1);

	if (length1 == 0) {
		interval->from  = sequence2;
		interval->until = sequence2;
		return;
	}

	const unsigned int length2 = string_length(sequence2);

	if (length2 == 0) {
		interval->from  = sequence1;
		interval->until = sequence1;
		return;
	}

	unsigned int *restrict *const restrict table
	= create_table(length1 + 1,
		       length2 + 1);

	do_longest_common_subsquence(interval,
				     sequence1 - 1,
				     length1,
				     sequence2 - 1,
				     length2,
				     table);

	free((void *) table);
}



int
main(void)
{
	struct Interval interval;
	static char buffer[128];
	char *restrict ptr;

	longest_common_subsquence(&interval,
				  "ABAB OOGABOOGA",
				  "BABA AAAAAAAA OOGA");

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
