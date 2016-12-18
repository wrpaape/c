#include "red_black_delete.h"

static inline struct RedBlackNode *
rb_do_replace_black_shallow(struct RedBlackNode *const restrict lchild,
			    struct RedBlackNode *const restrict rchild)
{
	if (rchild->color == RED) {
		/* rchild RED, lchild RED -> black height of 1 */
		rchild->color = BLACK;
		rchild->left  = lchild; /* lchild must be RED */

		return rchild;
	}

	/* black height of 2 */

	struct RedBlackNode *const restrict lrchild = lchild->right;

	if (lchild->color == RED) {
		/* rchild BLACK, lchild RED -> black height of 2 */
		lchild->color = BLACK;
		lchild->right = rchild;

		 /* lrchild must be non-NULL BLACK -> give to rchild */
		rchild->left = lrchild;

		lrchild->color = RED;

		return lchild;
	}

	/* rchild BLACK, lchild BLACK -> black height of 2 */

	if (lrchild == NULL) {
		lchild->right = rchild;

		rchild->color = RED;

		return lchild;
	}

	/* lrchild is RED, make new root */
	lchild->right = NULL;

	lrchild->color = BLACK;
	lrchild->left  = lchild;
	lrchild->right = rchild;

	return lrchild;
}

static inline bool
rb_replace_black(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;

	struct RedBlackNode *const restrict lchild = node->left;
	struct RedBlackNode *const restrict rchild = node->right;

	bool black_height_restored;

	if (lchild == NULL) {
		*tree = rchild;

		black_height_restored = (rchild != NULL);

		if (black_height_restored)
			rchild->color = BLACK; /* rchild is RED -> restore */

		return black_height_restored;
	}

	if (rchild == NULL) {
		*tree = lchild;
		lchild->color = BLACK;
		return true; /* lchild must be RED, -> BLACK -> restored */
	}

	/* find min successor, its parent, and its right child
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) {
		*tree = rb_do_replace_black_shallow(lchild,
						    rchild);
		return true; /* black height of 1 or 2 restorable in all cases */
	}

	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	if (replacement_child != NULL) {
		/* replacement was BLACK, replacement_child is RED */
		replacement_child->color = BLACK;

		*tree = replacement;
		replacement->left  = lchild;
		replacement->right = rchild;
		return true;
	}

	if (replacement->color == RED) {
		/* restore w/ no loss in black height in right subtree */
		replacement->color = BLACK;

		*tree = replacement;
		replacement->left  = lchild;
		replacement->right = rchild;
		return true;
	}


	/* TODO */

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
			(void) rb_replace_black(tree,
						grandparent);

		} else {
		}
	}
}
