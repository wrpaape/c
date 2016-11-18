#include <stdio.h>	/* printf */
#include <stdbool.h>	/* bool */

struct Interval {
	const unsigned int *restrict from;
	const unsigned int *restrict until;
};


static inline void
longest_increasing_subsequence(struct Interval *const restrict result,
			       const unsigned int *restrict sequence,
			       const unsigned int *restrict until)
{
	unsigned int length_longest;
	unsigned int length_current;
	unsigned int prev;
	unsigned int next;
	bool decreased;
	const unsigned int *restrict current;

	if (until <= sequence) {
		result->from  = until;
		result->until = until;
		return;
	}

	length_longest = 0;

	prev = *sequence;

	while (1) {
		current = sequence;

		while (1) {
			++sequence;

			if (sequence == until) {
				length_current = sequence - current;

				if (length_current > length_longest) {
					result->from  = current;
					result->until = sequence;
				}

				return;
			}

			next = *sequence;

			decreased = (next <= prev);

			prev = next;

			if (decreased) {
				length_current = sequence - current;

				if (length_current > length_longest) {
					length_longest = length_current;

					result->from  = current;
					result->until = sequence;
				}

				break;
			}
		}
	}
}

static inline void
print_array(const unsigned int *restrict from,
	    const unsigned int *const restrict until)
{
	if (until <= from) {
		puts("{}");
	} else {
		printf("{\n\t%u", *from);

		while (1) {
			++from;
			if (from == until)
				break;

			printf(", %u", *from);
		}

		puts("\n}");
	}

}

static inline void
do_lis(const unsigned int *const restrict from,
       const unsigned int *const restrict until)
{
	struct Interval interval;

	printf("\n\nfor sequence: ");

	print_array(from,
		    until);

	printf("the longest increasing subsquence is: ");

	longest_increasing_subsequence(&interval,
				       from,
				       until);

	print_array(interval.from,
		    interval.until);
}

#define UNTIL_PTR(SEQ) &SEQ[sizeof(SEQ) / sizeof(SEQ[0])]

#define SEQ(N) sequence ## N

#define DO_LIS(N) do_lis(&SEQ(N)[0], UNTIL_PTR(SEQ(N)))





int
main(void)
{

	const unsigned int sequence1[] = {
		0, 1, 2, 2, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6
	};

	DO_LIS(1);

	return 0;
}
