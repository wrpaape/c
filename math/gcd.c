#include <stdio.h>

int
gcd(const int x,
    const int y)
{
	if (y == 0)
		return x;

	return gcd(y, x % y);
}

int
gcd_iterative(int x,
	      int y)
{
	int tmp;

	while (y != 0) {
		tmp = y;
		y = x % y;
		x = tmp;
	}

	return x;
}


int
main(void)
{
	printf("gcd(120,  80): %d\n"
	       "gcd(80,  120): %d\n"
	       "gcd(13,   71): %d\n",
	       gcd_iterative(120, 80),
	       gcd_iterative(80, 120),
	       gcd_iterative(13, 71));
	return 0;
}
