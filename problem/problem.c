#include <stdio.h>

int term;   /* term used in two expressions */
int term_2; /* twice term */
int term_3; /* three times term */

int main() {
	term = 3 * 5;
	term_2 = 2 * term;
	term_3 = 3 * term;

  printf("Twice %d is %d\n", term, term_2);
	return (0);
}

