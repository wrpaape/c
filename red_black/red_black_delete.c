#include "red_black_delete.h"

void
red_black_delete(struct RedBlackNode *restrict *const restrict tree,
		 const struct Key *const restrict key)
{
	int64_t compare;
	struct RedBlackNode *restrict parent;

	struct RedBlackNode *const restrict grandparent = *tree;

	if (grandparent != NULL) {
		compare = key_compare(key,
				      grandparent->key);

		if (compare < 0) {
			parent = grandparent->left;

			if (parent == NULL) {
				grandparent->left = rb_new_node(key);

			} else {
				compare = key_compare(key,
						      parent->key);

				if (compare < 0)
					(void) rb_delete_ll(tree,
							    grandparent,
							    parent,
							    key);
				else if (compare > 0)
					(void) rb_delete_lr(tree,
							    grandparent,
							    parent,
							    key);
			}

		} else if (compare > 0) {
			parent = grandparent->right;

			if (parent == NULL) {
				grandparent->right = rb_new_node(key);

			} else {
				compare = key_compare(key,
						      parent->key);

				if (compare < 0)
					(void) rb_delete_rl(tree,
							    grandparent,
							    parent,
							    key);
				else if (compare > 0)
					(void) rb_delete_rr(tree,
							    grandparent,
							    parent,
							    key);
			}
		}
	}

}
