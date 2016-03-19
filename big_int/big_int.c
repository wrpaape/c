#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <limits.h>
#include "big_int.h"

#define UINT_BITS (sizeof(unsigned int) * CHAR_BIT)
#define LL_UI_SIZE_RATIO (sizeof(long long) / sizeof(unsigned int))

#if (UINT_MAX == 65535u)
#define CHARS_PER_DIGIT 5lu
#elif (UINT_MAX == 4294967295u)
#define CHARS_PER_DIGIT 10lu
#elif (UINT_MAX == 18446744073709551615u)
#define CHARS_PER_DIGIT 20lu
#else
#pragma GCC warning "unexpected value for 'UINT_MAX'! 'CHARS_PER_DIGIT' must be dynamically calculated"
#include <math.h>
#define CHARS_PER_DIGIT ((size_t) (floor(log10(UINT_MAX)) + 1lu))
#endif

int main(void)
{
	struct BigInt *big = new_big_int(ULLONG_MAX);

	printf("CHARS_PER_DIGIT: %lu\n", CHARS_PER_DIGIT);

	printf("to_s: %s\n", big_int_to_s(big));

	return 0;
}


struct BigInt *new_big_int(long long sml)
{
	/* struct BigInt *big = handle_malloc(sizeof(struct BigInt)); */
	struct BigInt *big = malloc(sizeof(struct BigInt));

	big->digits = malloc(sizeof(long long));

	//TODO: everything

	if (sml < 0) {
		big->sign = MINUS;
		sml	      = -sml;

	} else {
		big->sign = PLUS;
	}

	for (size_t i = 0; i < LL_UI_SIZE_RATIO; ++i)
		big->digits[i] = (unsigned int) (sml >> (UINT_BITS * i));


	printf("min:  %lld\n", LLONG_MIN);
	printf("lld:  %lld\n", (long long) big->digits);
	printf("u[0]: %u\n", (unsigned int) big->digits[0]);
	printf("u[1]: %u\n", (unsigned int) big->digits[1]);

	big->mag = 1;

	return big;
}


char *big_int_to_s(struct BigInt *big)
{
	/* char *dig_str = handle_malloc(sizeof(char) * */
	char *dig_str = malloc(sizeof(char) *
				      ((big->mag * CHARS_PER_DIGIT)
				       + 2lu));

	char *dig_char = dig_str;

	unsigned int *digit = big->digits;

	if (big->sign == MINUS) {
		*dig_char = '-';
		++dig_char;
	}

	//TODO: everything

	/* sprintf(dig_char, "%llu",(unsigned int) *digit); */
	sprintf(dig_char, "%llu", (unsigned long long) *big->digits);

	return dig_str;
}

#undef CHARS_PER_DIGIT
