#include <assert.h>	/* assert */
#include <stdio.h>	/* puts */


/* no static arrays (even though switch → jump table) */


int
section_closed(const char **string_ptr,
	       const char close)
{
	const char *string = *string_ptr;
	char token;

	while (1) {
		token = *string;

		switch (token) {
		case '[':
			++string;
			if (!section_closed(&string, ']'))
				return 0;
			break;
		case '(':
			++string;
			if (!section_closed(&string, ')'))
				return 0;
			break;
		case '{':
			++string;
			if (!section_closed(&string, '}'))
				return 0;
			break;
		case '<':
			++string;
			if (!section_closed(&string, '>'))
				return 0;
			break;

		case ']':
		case ')':
		case '}':
		case '>':
			*string_ptr = string;
		case '\0':	/* fall through */
			return close == token;

		default: /* continue */;
		}

		++string;
	}
}

int
brackets_balanced(const char *string)
{
	return section_closed(&string,
			      '\0');
}


int
main(void)
{
	int boolean;
	boolean = brackets_balanced("ooooooooga booga booooo");

	assert(boolean == 1);
	puts("passed");

	boolean = brackets_balanced("[ooga{booga<looga(snooga)>}booga]snooga");

	assert(boolean == 1);
	puts("passed");

	boolean = brackets_balanced("[ooga{booga<looga(snooga)<>}booga]snooga");

	assert(boolean == 0);
	puts("passed");

	return 0;
}
