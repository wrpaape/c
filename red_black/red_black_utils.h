#ifndef RED_BLACK_UTILS_H_
#define RED_BLACK_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "key_utils.h" /* struct Key, get_key, key_compare, exit_on_* */
#include <stdbool.h>   /* bool */


/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
enum Color {
	RED,
	BLACK
};

struct RedBlackNode {
	const struct Key *key;
	enum Color color;
	struct RedBlackNode *left;
	struct RedBlackNode *right;
};



bool
red_black_find(const struct RedBlackNode *restrict node,
	       const struct Key *const restrict key);

void
red_black_insert(struct RedBlackNode *restrict *restrict tree,
		 const struct Key *const restrict key);

void
red_black_print(const struct RedBlackNode *const restrict tree);

#endif /* ifndef RED_BLACK_UTILS_H_ */
