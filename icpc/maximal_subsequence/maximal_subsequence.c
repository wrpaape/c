#include <stdio.h>	/* printf */

struct Subsequence {
	int total;
	const int *restrict from;
	const int *restrict until;
};


static inline void
maximal_subsequence(struct Subsequence *const restrict result,
		    const int *restrict sequence,
		    const int *restrict until)
{
	int *restrict acc_from;
	int *restrict max_from;
	int *restrict acc_upto;
	int *restrict max_upto;
	int value;
	int next_total;
	int acc_total;
	int max_total;


	if (until <= sequence) {
		result->total = 0;
		result->from  = until;
		result->until = until;
	}

	max_from  = sequence;
	acc_from  = sequence;
	max_upto  = sequence;
	acc_upto  = sequence;
	max_total = *sequence;
	acc_total = max_total;

	++sequence;

	while (sequence < until) {

		value = *sequence;

		next_total = acc_total + value;

		if (value > next_total) {
			acc_total = value;
			acc_from  = sequence;

		} else {
			acc_total = next_total;
		}

		acc_upto = sequence;

		if (acc_total > max_total) {
			max_from  = acc_from;
			max_upto  = acc_upto;
			max_total = value;


		}


		++sequence;
	}


	result->total = max_total;
	result->from  = max_from;
	result->until = max_upto + 1l;
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
	struct Subsequence subsequence;

	printf("\n\nfor sequence: ");

	print_array(from,
		    until);

	printf("the maximum subsquence is: ");

	maximal_subsequence(&subsequence,
			    from,
			    until);

	print_array(interval.from,
		    interval.until);

	printf("total: %d\n",
	       interval.total);
}


#define UNTIL_PTR(SEQ) &SEQ[sizeof(SEQ) / sizeof(SEQ[0])]
#define SEQ(N) sequence ## N
#define DO_MS(N) do_ms(&SEQ(N)[0], UNTIL_PTR(SEQ(N)))

int
main(void)
{

	const int sequence1[] = {
		-2, 1, -3, 4, -1, 2, 1, -5, 4
	};

	DO_MDO_MS(1);

	return 0;
}
