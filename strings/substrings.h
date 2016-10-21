#ifndef SUBSTRINGS_H_
#define SUBSTRINGS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>	/* malloc, realloc, free, exit */
#include <stdio.h>	/* fprintf */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror */
#include <stdbool.h>	/* bool */
#include <unistd.h>	/* write, size_t */
#include <stdint.h>	/* uint64_t */


/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define EXIT_ON_FAILURE(FORMAT, ...)					\
do {									\
	fprintf(stderr,							\
		"\n\e[31m\e[5mERROR:\e[25m\t\e[4m" FORMAT "\e[24m\n\n"	\
		"errno: %s\n\n"						\
		"file:  %s\n\n"						\
		"func:  %s\n\n"						\
		"line:  %d\e[0m\n",					\
		##__VA_ARGS__,						\
		strerror(errno), __FILE__, __func__, __LINE__);		\
	exit(EXIT_FAILURE);						\
	__builtin_unreachable();					\
} while (0)

#define LIKELY(BOOL)   __builtin_expect(BOOL, true)
#define UNLIKELY(BOOL) __builtin_expect(BOOL, false)

#define WRITE_OUTPUT(STRING, SIZE)					\
if (UNLIKELY(write(STDOUT_FILENO,					\
		   STRING,						\
		   SIZE) < 0))						\
	EXIT_ON_FAILURE("failed to print string \"%s\"",		\
			STRING)


#define WRITE_OUTPUT_LITERAL(STRING)					\
WRITE_OUTPUT(STRING, sizeof(STRING) - 1lu)

#endif /* ifndef SUBSTRINGS_H_ */
