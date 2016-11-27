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
	struct Key *key;
	enum Color color;
	struct RedBlackNode *parent;
	struct RedBlackNode *less;
	struct RedBlackNode *more;
};



bool
red_black_is_member(const struct RedBlackNode *restrict node,
		    const struct Key *const restrict key);


#endif /* ifndef RED_BLACK_UTILS_H_ */
