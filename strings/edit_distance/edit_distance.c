#include <stdlib.h>	/* malloc, free, exit */
#include <string.h>	/* string length */
#include <stdio.h>	/* printf */

struct Substring {
	const char *from;
	const char *upto;
};

unsigned int
do_edit_distance_exponential(struct Substring *const restrict substring1,
			     struct Substring *const restrict substring2)
{
	unsigned int min_distance;
	unsigned int distance;

	if (substring1->from > substring1->upto)
		return substring2->upto + 1 - substring2->from;

	if (substring2->from > substring2->upto)
		return substring1->upto + 1 - substring1->from;

	const unsigned int no_match
	= (*(substring1->upto) != *(substring2->upto));

	--(substring1->upto);
	--(substring2->upto);

	min_distance = do_edit_distance_exponential(substring1,
						    substring2) + no_match;

	++(substring1->upto);

	distance = do_edit_distance_exponential(substring1,
						substring2) + 1;

	if (distance < min_distance)
		min_distance = distance;

	--(substring1->upto);
	++(substring2->upto);

	distance = do_edit_distance_exponential(substring1,
						substring2) + 1;

	++(substring1->upto);

	if (distance < min_distance)
		min_distance = distance;

	return min_distance;
}

static inline void
substring_init(struct Substring *const restrict substring,
	       const char *const restrict string)
{
	substring->from = string;
	substring->upto = string + strlen(string);
}



static inline unsigned int
edit_distance_exponential(const char *const string1,
			  const char *const string2)
{
	struct Substring substring1;
	struct Substring substring2;

	substring_init(&substring1,
		       string1);

	substring_init(&substring2,
		       string2);

	return do_edit_distance_exponential(&substring1,
					    &substring2);
}

static inline unsigned int *restrict *restrict
create_history(const size_t size1,
	       const size_t size2)
{
	unsigned int *restrict *restrict row;
	unsigned int *restrict cell;
	unsigned int *restrict cell_until;


	unsigned int *restrict *const restrict history
	= malloc(  (sizeof(unsigned int *) * size1)
		 + (sizeof(unsigned int)   * (size1 * size2)));

	if (history == NULL) {
		perror("malloc");
		exit(1);
	}

	row = history;

	unsigned int *restrict *const restrict row_until = row + size1;

	cell = (unsigned int *restrict) row_until;

	while (1) {
		*row = cell;

		++row;
		if (row == row_until)
			return history;

		cell += size2;
	}

	return history;
}

static inline unsigned int
do_edit_distance(unsigned int *const restrict *const restrict history,
		 const char *const string1_m1,
		 const size_t length1,
		 const char *const string2_m1,
		 const size_t length2)
{
	size_t i;
	size_t i_m1;
	size_t j;
	size_t j_m1;

	unsigned int no_match;
	unsigned int distance;
	unsigned int min_distance;

	j = 0;

	do {
		history[0][j] = j;
		++j;
	} while (j < length2);


	i_m1 = 0;
	i    = 1;

	while (1) {
		history[i][0] = i;

		j_m1 = 0;
		j    = 1;

		while (1) {
			no_match = (string1_m1[i] != string2_m1[j]);

			min_distance = history[i_m1][j_m1] + no_match;

			distance     = history[i_m1][j] + 1;

			if (distance < min_distance)
				min_distance = distance;

			distance     = history[i][j_m1] + 1;

			if (distance < min_distance)
				min_distance = distance;

			history[i][j] = min_distance;

			if (j == length2)
				break;

			j_m1 = j;
			++j;
		}

		if (i == length1)
			return history[i][j];

		i_m1 = i;
		++i;
	}
}


static inline unsigned int
edit_distance(const char *const string1,
	      const char *const string2)
{
	const size_t length1 = strlen(string1);
	const size_t length2 = strlen(string2);

	if (length1 == 0) {
		return (unsigned int) length2;

	} else if (length2 == 0) {
		return (unsigned int) length1;
	}


	unsigned int *restrict *const restrict history
	= create_history(length1 + 1,
			 length2 + 1);

	const unsigned int distance = do_edit_distance(history,
						       string1 - 1,
						       length1,
						       string2 - 1,
						       length2);

	free((void *) history);

	return distance;
}





int
main(void)
{
	printf("edit_distance: %u\n",
	       edit_distance("you should not",
			     "thou shall not"));

	return 0;
}
