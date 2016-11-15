#include <stdlib.h>	/* malloc, free */
#include <string.h>	/* string length */
#include <stdio.h>	/* printf */

struct Substring {
	const char *from;
	const char *upto;
};

unsigned int
do_edit_distance(struct Substring *const restrict substring1,
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

	min_distance = do_edit_distance(substring1,
					substring2) + no_match;

	++(substring1->upto);

	distance = do_edit_distance(substring1,
				    substring2) + 1;

	if (distance < min_distance)
		min_distance = distance;

	--(substring1->upto);
	++(substring2->upto);

	distance = do_edit_distance(substring1,
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
edit_distance(const char *const string1,
	      const char *const string2)
{
	struct Substring substring1;
	struct Substring substring2;

	substring_init(&substring1,
		       string1);

	substring_init(&substring2,
		       string2);

	return do_edit_distance(&substring1,
				&substring2);
}


int
main(void)
{
	printf("edit_distance: %u\n",
	       edit_distance("you should n",
			     "thou shall n"));

	return 0;
}
