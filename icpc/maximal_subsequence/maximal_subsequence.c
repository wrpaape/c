#include <stdio.h>	/* printf */

struct Interval {
	const int *restrict from;
	const int *restrict until;
};

static inline void
maximal_subsequence(struct Interval *const restrict result,
		    const int *restrict sequence,
		    const int *restrict until)
{
}


static inline void
print_array(const int *restrict from,
	    const int *const restrict until)
{
	if (until <= from) {
		puts("{}");
	} else {
		printf("{\n\t%d", *from);

		while (1) {
			++from;
			if (from == until)
				break;

			printf(", %d", *from);
		}

		puts("\n}");
	}

}


static inline void
do_ms(const int *const restrict from,
      const int *const restrict until)
{
	struct Interval interval;

	printf("\n\nfor sequence: ");

	print_array(from,
		    until);

	printf("the maximum subsquence is: ");

	maximal_subsequence(&interval,
			    from,
			    until);

	print_array(interval.from,
		    interval.until);
}

#define UNTIL_PTR(SEQ) &SEQ[sizeof(SEQ) / sizeof(SEQ[0])]

#define SEQ(N) sequence ## N

#define DO_MS(N) do_ms(&SEQ(N)[0], UNTIL_PTR(SEQ(N)))





int
main(void)
{

	const int sequence1[] = {
		0, 1, 2, 2, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6
	};

	DO_MDO_MS(1);

	return 0;
}
