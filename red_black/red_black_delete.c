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

/* attempt to restore black height in the replacement (right) tree, if can't,
 * restore balance and return false */
static inline bool
rb_replace_black_rtree(struct RedBlackNode *restrict *restrict root,
		       struct RedBlackNode *const restrict *restrict stack_ptr,
		       struct RedBlackNode *restrict parent)
{
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict grandchild;
	struct RedBlackNode *restrict great_grandchild;
	struct RedBlackNode *restrict grandparent;

	rchild     = parent->right;
	grandchild = rchild->left;

	if (parent->color == RED) {
		parent->color = BLACK;
		/* rchild must be BLACK */

		if (grandchild == NULL) {
			/* balance restored, not black height */
			rchild->color = RED;

		} else {
			grandparent = *stack_ptr;

			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			/* balance AND black height restored */
			*root = grandchild;

			grandchild->color = BLACK;
			grandchild->left  = parent;
			grandchild->right = rchild;;

			parent->right == NULL;
			return true;
		}

	} else if (rchild->color == RED) {
		/* grandchild is BLACK, not NULL */
		great_grandchild = grandchild->left;

		if (great_grandchild == NULL) {
			grandchild->left = parent; /* put parent on min */

			parent->color = RED;
			parent->right = NULL;

			grandparent = *stack_ptr;

			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			/* balance AND black height restored */
			*root = rchild;

			rchild->color = BLACK;
			return true;
		}



	} else {
	}

}


static inline bool
rb_replace_black(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement_child;
	struct RedBlackNode *restrict replacement;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;

	/* can handle trees of upto at least 1,000,000,000 nodes */
	struct RedBlackNode *const restrict replacement_stack[81];
	struct RedBlackNode *const restrict *restrict replacement_stack_ptr;

	lchild = node->left;
	rchild = node->right;

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

	/* find min successor, its parent, its right child, and its ancestor
	 * stack of rchild->left->left-> ... ->grandparent
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) {
		*tree = rb_replace_black_shallow(lchild,
						 rchild);
		return true; /* black height of 1 or 2 always restorable */
	}

	replacement_stack_ptr  = &replacement_stack[0];
	*replacement_stack_ptr = NULL;

	replacement_parent = rchild;

	while (1) {
		replacement_child = replacement->left;
		if (replacement_child == NULL)
			break;

		++replacement_stack_ptr;
		*replacement_stack_ptr = replacement_parent;

		replacement_parent = replacement;
		replacement        = replacement_child;
	}

	replacement_child	 = replacement->right;
	replacement_parent->left = replacement_child; /* pop replacement */

	if (   rb_replace_black_simple(rchild,
				       replacement,
				       replacement_child)
	    || rb_replace_black_rtree(&rchild,
				      replacement_stack_ptr,
				      replacement_parent)) {
		/* rchild, replacement, or replacement_child was colored from
		 * RED -> BLACK to restore black height in replacement
		 * subtree
		 *
		 * OR
		 *
		 * black height restored through recoloring, rotation of
		 * ancestors of replacement
		 *
		 * replacement guaranteed BLACK */
		*tree = replacement;
		replacement->left  = lchild;
		replacement->right = rchild;
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
