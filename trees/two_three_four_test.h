#ifndef TWO_THREE_FOUR_TEST_H_
#define TWO_THREE_FOUR_TEST_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "two_three_four.h"	/* 2-3-4 tree, helper macros */
#include <sys/types.h>		/* read */
#include <sys/uio.h>		/* read */
#include <unistd.h>		/* read */


/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define READ_INPUT_SIZE(SIZE_READ, BUFFER, SIZE)			\
do {									\
	SIZE_READ = read(STDIN_FILENO,					\
			 BUFFER,					\
			 SIZE);						\
	if (UNLIKELY(SIZE_READ < 0))					\
		EXIT_ON_FAILURE("failed to read input size %zu",	\
				SIZE);					\
} while (0)


#define WRITE_OUTPUT(STRING, SIZE)					\
if (UNLIKELY(write(STDOUT_FILENO,					\
		   STRING,						\
		   SIZE) < 0))						\
	EXIT_ON_FAILURE("failed to print string %s",			\
			STRING)


#define WRITE_OUTPUT_LITERAL(STRING)					\
WRITE_OUTPUT(STRING, sizeof(STRING) - 1lu)


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline char *
get_input(void);


/* tree compare function */
int
compare_strings(void *value1,
		void *value2);

/* tree stringify function */
char *
put_string(char *restrict buffer,
	   void *pointer);


#endif /* ifndef TWO_THREE_FOUR_TEST_H_ */
