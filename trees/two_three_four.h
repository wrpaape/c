#ifndef TWO_THREE_FOUR_H_
#define TWO_THREE_FOUR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>	/* malloc, realloc, free, exit */
#include <stdio.h>	/* fprintf */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror */


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

#define LIKELY(BOOL)   __builtin_expect(BOOL, 1)
#define UNLIKELY(BOOL) __builtin_expect(BOOL, 0)



/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct TwoThreeFourTuple {
	void *key;
	void *value;
};

struct TwoThreeFourNode {
	struct TwoThreeFourTuple tuples[3];
	struct TwoThreeFourNode *children[4];
	unsigned int count_children;
};

struct TwoThreeFourAlloc {
	struct TwoThreeFourNode *base;
	struct TwoThreeFourNode *current;
	struct TwoThreeFourNode *until;
	size_t size;
};

typedef int
Comparator(void *value1,
	   void *value2);

typedef char *
Stringifier(char *restrict buffer,
	    void *value);

struct TwoThreeFourTree {
	struct TwoThreeFourNode *root;
	Comparator *compare;
	Stringifier *stringify_key;
	Stringifier *stringify_value;
	struct TwoThreeFourAlloc alloc;
};


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
two_three_four_alloc_init(struct TwoThreeFourAlloc *const restrict alloc);

static inline void
two_three_four_alloc_expand(struct TwoThreeFourAlloc *const restrict alloc);

static inline struct TwoThreeFourNode *
two_three_four_alloc_pop(struct TwoThreeFourAlloc *const restrict alloc);


/* API
 * ────────────────────────────────────────────────────────────────────────── */
void
two_three_four_tree_init(struct TwoThreeFourTree *const restrict tree,
			 Comparator *const compare,
			 Stringifier *const stringify_key,
			 Stringifier *const stringify_value);


inline void
two_three_four_tree_free(struct TwoThreeFourTree *const restrict tree)
{
	free(tree->alloc.base);
}



#endif /* ifndef TWO_THREE_FOUR_H_ */
