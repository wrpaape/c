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

enum RedBlackCorrectState {
	CORRECT_PREV,
	CORRECT_THIS,
	CORRECT_DONE
};

struct RedBlackNode {
	const struct Key *key;
	enum Color color;
	struct RedBlackNode *left;
	struct RedBlackNode *right;
};

#endif /* ifndef RED_BLACK_UTILS_H_ */
