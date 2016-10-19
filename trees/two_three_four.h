#ifndef TWO_THREE_FOUR_H_
#define TWO_THREE_FOUR_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>	/* malloc, realloc, free, exit */
#include <stdio.h>	/* fprintf */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror */
#include <stdbool.h>	/* bool */


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



/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef int
Comparator(void *key1,
	   void *key2);

typedef char *
StringPutter(char *restrict buffer,
	     void *value);

struct TwoThreeFourTuple {
	void *key;
	void *value;
};

enum TwoThreeFourNodeType {
	TTF_TWO_NODE,
	TTF_THREE_NODE,
	TTF_FOUR_NODE
};

struct TwoThreeFourNode {
	enum TwoThreeFourNodeType type;
	struct TwoThreeFourTuple tuples[3];
	struct TwoThreeFourNode *children[4];
};

struct TwoThreeFourNodeBlock {
	struct TwoThreeFourNode *base;
	struct TwoThreeFourNode *current;
	struct TwoThreeFourNode *until;
};

struct TwoThreeFourAlloc {
	struct TwoThreeFourNodeBlock block;
	struct TwoThreeFourNode *free;
	struct TwoThreeFourNode *active;
};

struct TwoThreeFourCache {
	struct TwoThreeFourTuple input;
	Comparator *compare;
	struct TwoThreeFourAlloc alloc;
};

struct TwoThreeFourStringifier {
	StringPutter *put_key;
	StringPutter *put_value;
};


struct TwoThreeFourTree {
	struct TwoThreeFourNode *root;
	struct TwoThreeFourCache cache;
	struct TwoThreeFourStringifier stringifier;
};



/* API
 * ────────────────────────────────────────────────────────────────────────── */
void
two_three_four_tree_init(struct TwoThreeFourTree *const restrict tree,
			 Comparator *const compare,
			 StringPutter *const put_key,
			 StringPutter *const put_value);

void
two_three_four_tree_insert(struct TwoThreeFourTree *const restrict tree,
			   void *key,
			   void *value);
bool
two_three_four_tree_find(struct TwoThreeFourTree *const restrict tree,
			 void *key,
			 void **const restrict value);

bool
two_three_four_tree_update(struct TwoThreeFourTree *const restrict tree,
			   void *key,
			   void *value);

void
two_three_four_tree_delete(struct TwoThreeFourTree *const restrict tree,
			   void *key);

void
two_three_four_tree_free(struct TwoThreeFourTree *const restrict tree);



#endif /* ifndef TWO_THREE_FOUR_H_ */
