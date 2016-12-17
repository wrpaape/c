#include "red_black_delete.h"

static inline struct RedBlackNode *
rb_replace(struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict *restrict prev_ptr;
	struct RedBlackNode *restrict min_successor;
	struct RedBlackNode *restrict next;

	struct RedBlackNode *const restrict lchild = node->left;
	struct RedBlackNode *const restrict rchild = node->right;

	if (lchild == NULL)
		return rchild;

	if (rchild == NULL)
		return lchild;

	min_successor = rchild->left;

	if (min_successor == NULL) {
		rchild->left = lchild;
		return rchild;
	}

	prev_ptr = &rchild->left;

	while (1) {
		next = min_successor->left;
		if (next == NULL)
			break;

		prev_ptr = &min_successor->left;
		min_successor = next;
	}

	min_successor->left = lchild;

	next = min_successor->right;

	min_successor->right = rchild;

	*prev_ptr = next;

	if (next != NULL)
		next->color = BLACK;	/* min_sucessor is BLACK, next is RED */

	return min_successor;

}

static inline void
rb_delete_root(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict root)
{
	struct RedBlackNode *restrict grandchild;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	lchild = root->left;

	if (lchild == NULL) {
		rchild = root->right;

		if (rchild != NULL)
			rchild->color = BLACK;

		*tree = rchild;

	} else if (lchild->color == RED) {
		grandchild = lchild->left;

		if (grandchild == NULL) {
			lchild->color = BLACK;
			*tree = lchild;

		} else {
			rchild = root->right;

			if (rchild->color == RED) {

			} else {

			}

		}

	} else {
		rchild = root->right;
		/* must be non-NULL */
		if (rchild->color == RED) {



		} else {

		}
	}
}

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

		if (compare == 0) {
			rb_delete_root(tree,
				       grandparent);
		} else {
		}
	}
}
