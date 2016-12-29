#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "key_utils.h" /* struct Key, get_key, key_compare, exit_on_* */
#include <pthread.h>   /* pthread_rwlock_t */
#include <stdbool.h>   /* bool */
#include "red_black_comparator.h"
#include "red_black_allocator.h" /* struct RedBlack Node */



/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
enum RedBlackCorrectState {
	CORRECT_PREV,
	CORRECT_THIS,
	CORRECT_DONE
};




struct RedBlackTree {
	pthread_rwlock_t lock;
	struct RedBlackNode *root;
	RedBlackComparator comparator;
	struct RedBlackAllocator allocator;
};

#endif /* ifndef RED_BLACK_TREE_H_ */
