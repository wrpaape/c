#ifndef TWO_THREE_FOUR_H_
#define TWO_THREE_FOUR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>	/* malloc, realloc, free, exit */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror */
#include <sys/types.h>	/* read */
#include <sys/uio.h>	/* read */
#include <unistd.h>	/* read */


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
} while (0)

#define LIKELY(BOOL)   __builtin_expect(BOOL, 1)
#define UNLIKELY(BOOL) __builtin_expect(BOOL, 0)

#define MALLOC(POINTER, SIZE)						\
do {									\
	POINTER = malloc(SIZE);						\
	if (UNLIKELY(POINTER == NULL))					\
		EXIT_ON_FAILURE("failed to malloc %zu bytes for '"	\
				#POINTER "'",				\
				SIZE);					\
} while (0)

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



/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct TwoThreeFourNode {
	void *values[3];
	struct TwoThreeFourNode *children;
	unsigned int count_values;
};

struct TwoThreeFourAlloc {
	struct TwoThreeFourNode *base;
	struct TwoThreeFourNode *from;
	struct TwoThreeFourNode *until;
	size_t size;
};

struct TwoThreeFourTree {
	struct TwoThreeFourNode *root;
	int (*compare)(void *value1
		       void *value2);
	struct TwoThreeFourAlloc alloc;
};


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline char *
get_input(void)
{
	static char buffer[4096];
	static char *restrict next	 = &buffer[0];
	static const char *restrict last = next + 4095;
	char *restrict input;
	ssize_t size_read;

	READ_INPUT_SIZE(size_read,
			next,
			last - next);

	input = next;

	next += size_read;

	if (next == last) {
		WRITE_OUTPUT_LITERAL("OUT OF SPACE\n");
		exit(0);
	}

	return input;
}


#endif /* ifndef TWO_THREE_FOUR_H_ */
