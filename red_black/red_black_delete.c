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
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict grandchild;
	struct RedBlackNode *restrict great_grandchild;
	struct RedBlackNode *restrict grandparent;
	struct RedBlackNode *restrict great_grandparent;
	struct RedBlackNode *restrict uncle;

	grandparent = *stack_ptr;
	rchild      = parent->right;
	grandchild  = rchild->left;

	if (parent->color == RED) {
		parent->color = BLACK;
		/* rchild must be BLACK */
		if (grandchild == NULL) {
			rchild->color = RED;
			grandchild = rchild->right;

			if (grandchild != NULL) {
				/* grandchild must be RED leaf */
				if (grandparent != NULL) /* parent != root */
					root = &grandparent->left;

				*root = rchild;

				rchild->left  = parent;
				rchild->right = grandchild;

				parent->right = NULL;

				grandchild->color = BLACK;
				return true; /* balance AND black height restored */
			}
			/* !! balance restored, NOT black height !! */
		} else {
			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			*root = grandchild;

			grandchild->left  = parent;
			grandchild->right = rchild;

			parent->right = NULL;

			rchild->left = NULL;
			return true; /* balance AND black height restored */
		}

	} else if (rchild->color == RED) {
		 /* can balance AND restore black height */
		parent->right = NULL;

		if (grandparent != NULL) /* parent != root */
			root = &grandparent->left;

		/* grandchild is BLACK, not NULL */
		great_grandchild = grandchild->left;

		if (great_grandchild == NULL) {
			grandchild->left = parent; /* put parent on min */

			parent->color = RED;

			*root = rchild;

			rchild->color = BLACK;

		} else {
			grandchild->left = NULL; /* bring ggchild to root */

			*root = great_grandchild;

			great_grandchild->color = BLACK;
			great_grandchild->left  = parent;
			great_grandchild->right = rchild;
		}
		return true; /* balance AND black height restored */

	} else if (grandchild == NULL) {
		grandchild = rchild->right;

		if (grandchild != NULL) {
			/* grandchild must be RED leaf */
			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			*root = rchild;

			rchild->left  = parent;
			rchild->right = grandchild;

			parent->right = NULL;

			grandchild->color = BLACK;
			return true; /* balance AND black height restored */
		}

		rchild->color = RED;
		/* !! balance restored, NOT black height !! */
	} else {
		/* granchild is a RED leaf, make new root */
		if (grandparent != NULL) /* parent != root */
			root = &grandparent->left;

		*root = grandchild;

		grandchild->color = BLACK;
		grandchild->left  = parent;
		grandchild->right = rchild;

		parent->right = NULL;

		rchild->left = NULL;
		return true; /* balance AND black height restored */
	}

	/* grandparent->parent->rchild balanced, but are deficient 1 black
	 * height -> unwind stack to look for opportunity to correct
	 * ────────────────────────────────────────────────────────────────── */
	if (grandparent == NULL)
		return false; /* unwound all the way to root w/o correction */

	while (1) {
		--stack_ptr;
		great_grandparent = *stack_ptr;



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
		 * black height restored through recoloring and/or rotation of
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
