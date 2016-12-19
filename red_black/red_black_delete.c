#include "red_black_delete.h"

static inline bool
rb_replace_black_rb(struct RedBlackNode *const restrict lchild,
		    struct RedBlackNode *const restrict rchild,
		    struct RedBlackNode *const restrict replacement)

static inline struct RedBlackNode *
rb_replace_black_shallow(struct RedBlackNode *const restrict lchild,
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

/* return true if simple change can be made to restore original black height in
 * replacement (right) subtree of deleted node (and do change) */
static inline bool
rb_replace_black_simple(struct RedBlackNode *const restrict rchild,
			struct RedBlackNode *const restrict replacement,
			struct RedBlackNode *const restrict replacement_child)
{
	if (replacement_child != NULL) /* replacement is BLACK, child is RED  */
		replacement_child->color = BLACK;
	else if (replacement->color == RED)
		replacement->color	 = BLACK;
	else if (rchild->color == RED)
		rchild->color		 = BLACK;
	else
		return false;

	return true;
}

/* attempt to restore black height in the replacement tree */
static inline bool
rb_replace_black_rtree(struct RedBlackNode *restrict *const restrict tree,
		       struct RedBlackNode *const restrict parent,
		       struct RedBlackNode *const restrict replacement)
{
}


static inline bool
rb_replace_black(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict *restrict replacement_tree;
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;

	struct RedBlackNode *const restrict lchild = node->left;
	struct RedBlackNode *const restrict rchild = node->right;

	if (lchild == NULL) {
		*tree = rchild;

		const bool black_height_restored = (rchild != NULL);

		if (black_height_restored)
			rchild->color = BLACK; /* rchild is RED -> restore */

		return black_height_restored;
	}

	if (rchild == NULL) {
		*tree = lchild;
		lchild->color = BLACK;
		return true; /* lchild must be RED, -> BLACK -> restored */
	}

	/* find min successor, its parent (and maybe pointer to parent), and
	 * its right child
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) {
		*tree = rb_replace_black_shallow(lchild,
						 rchild);
		return true; /* black height of 1 or 2 always restorable */
	}

	replacement_tree   = &rchild;
	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		replacement_tree   = &replacement_parent->left;
		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child	 = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	if (   rb_replace_black_simple(rchild,
				       replacement,
				       replacement_child)
		/* rchild, replacement, or replacement_child was colored from
		 * RED -> BLACK to restore black height in replacement
		 * subtree
		 * replacement guaranteed BLACK */
	    || rb_replace_black_rtree(replacement_tree,
				      replacement_parent,
				      replacement)) {
		replacement->left  = lchild;
		replacement->right = rchild;
		*tree = replacement;
		return true;
	}

	/* rchild and replacement are BLACK, right subtree is valid but
	 * deficient 1 black height */

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
