#include "substrings.h"


static inline void
print_matches(const char *restrict substring,
	      const char *restrict string)
{
}

int
main(int argc,
     char *argv[])
{
	if (argc == 3)
		print_matches(argv[1],
			      argv[2]);
	else
		WRITE_OUTPUT_LITERAL("substrings <substring> <string>\n");

	return 0;
}
