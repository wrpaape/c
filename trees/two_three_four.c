#include "two_three_four.h"

int
compare_strings(void *value1,
		void *value2)
{
	char token1;
	char token2;
	const char *restrict string1 = (const char *restrict) value1;
	const char *restrict string2 = (const char *restrict) value2;

	while (1) {
		token1 = *string1;
		token2 = *string2;

		if (token1 != token2)
			return token1 - token2;

		if (token1 == '\0')
			return 0;

		++string1;
		++string2;
	}
}








int
main(void)
{
	return 0;
}
