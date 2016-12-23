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

/* static inline void */
/* rb_replace_black_unwind_bprn(struct RedBlackNode *restrict *const restrict root, */
/* 			     struct RedBlackNode *const restrict parent, */
/* 			     struct RedBlackNode *const restrict lchild) */
/* { */

/* 	llgrandchild = lchild->left; */
/* 	lrgrandchild = lchild->right; */

/* 	if (lrgrandchild->color == RED) { */
/* 		/1* lchild is root, no need to update *1/ */
/* 		lrgrandchild->color = BLACK; */

/* 		lchild->color = RED; /1* lchild must have been BLACK *1/ */
/* 		lchild->left  = parent; */

/* 		parent->right = llgrandchild; */
/* 	} else if (llgrandchild->color == RED) { */
/* 		*root = llgrandchild; /1* make llgranchild new root *1/ */

/* 		parent->right      = llgrandchild->left; */
/* 		llgrandchild->left = parent; */

/* 		lchild->left        = llgrandchild->right; */
/* 		llgrandchild->right = lchild; */

/* 	} else { */
/* 		/1* lchild is root, no need to update *1/ */
/* 		lchild->left = parent; */

/* 		parent->color = RED; /1* parent and parent->left were BLACK *1/ */
/* 		parent->right = llgrandchild; */
/* 	} */
/* } */


/* /1* parent is RED, node is BLACK */
/*  * nodes are named according to their relation to 'node' *1/ */
/* static inline struct RedBlackNode * */
/* rb_replace_black_unwind_rpbn(struct RedBlackNode *const restrict parent, */
/* 			     struct RedBlackNode *const restrict node) */
/* { */
/* 	struct RedBlackNode *const restrict lchild = node->left; */

/* 	if (lchild->color == RED) { */
/* 		parent->color = BLACK; */
/* 		parent->right = lchild->left; */
/* 		lchild->left  = parent; */

/* 		node->left    = lchild->right; */
/* 		lchild->right = node; */

/* 		return lchild; /1* lchild is the new root *1/ */
/* 	} */

/* 	parent->right = lchild; */

/* 	node->left = parent; */

/* 	return node; /1* node is the new root *1/ */
/* } */


/* unwind min successor stack */
static inline bool
rb_replace_black_unwind(struct RedBlackNode *restrict *restrict root,
			struct RedBlackNode *const restrict *restrict stack_ptr,
			struct RedBlackNode *restrict parent)
{
	struct RedBlackNode *restrict grandparent;
	struct RedBlackNode *restrict node;
	struct RedBlackNode *restrict lchild;
	struct RedBlackNode *restrict rchild;
	struct RedBlackNode *restrict llgrandchild;
	struct RedBlackNode *restrict lrgrandchild;

	while (parent != NULL) {
		--stack_ptr;
		grandparent = *stack_ptr;

		node   = parent->right;
		lchild = node->left;

		if (node->color == RED) {
			/* parent is BLACK, node is RED
			 * ────────────────────────────────────────────────── */
			node->color = BLACK;

			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			*root = node; /* node is the new root */

			/* node->left MAY be replaced */
			rb_replace_black_unwind_bprn(&node->left,
						     parent,
						     node->left);
			llgrandchild = lchild->left;
			lrgrandchild = lchild->right;

			if (lrgrandchild->color == RED) {
				/* no need to update lchild */
				lrgrandchild->color = BLACK;

				/* lchild must have been BLACK */
				lchild->color = RED;
				lchild->left  = parent;

				parent->right = llgrandchild;
			} else if (llgrandchild->color == RED) {
				/* make llgrandchild new lchild */
				node->left = llgrandchild;

				parent->right      = llgrandchild->left;
				llgrandchild->left = parent;

				lchild->left        = llgrandchild->right;
				llgrandchild->right = lchild;

			} else {
				/* lchild is root, no need to update */
				lchild->left = parent;

				/* parent and parent->left were BLACK */
				parent->color = RED;
				parent->right = llgrandchild;
			}

			return true; /* always completely restorable */

		} else if (parent->color == RED) {
			/* parent is RED, node is BLACK
			 * ────────────────────────────────────────────────── */
			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			/* root will ALWAYS be replaced */
			if (lchild->color == RED) {
				*root = lchild; /* lchild is new root */

				parent->color = BLACK;
				parent->right = lchild->left;
				lchild->left  = parent;

				node->left    = lchild->right;
				lchild->right = node;

			} else {
				*root = node; /* node is new root */

				node->left = parent;

				parent->right = lchild;
			}

			return true; /* always completely restorable */

		/* parent is BLACK, node is BLACK
		 * ────────────────────────────────────────────────────────── */
		} else if (lchild->color == RED) {
			lchild->color = BLACK;

			if (grandparent != NULL) /* parent != root */
				root = &grandparent->left;

			*root = lchild; /* lchild is new root */

			parent->right = lchild->left;
			lchild->left  = parent;

			node->left    = lchild->right;
			lchild->right = node;

			return true; /* always completely restorable */

		} else {
			rchild = node->right;

			if (rchild->color == RED) {
				rchild->color = BLACK;

				if (grandparent != NULL) /* parent != root */
					root = &grandparent->left;

				*root = node; /* node is new root */

				node->left = parent;

				parent->right = lchild;

				return true; /* always completely restorable */
			}
		}
		/* parent, sibling (parent->left), node (parent->right), lchild
		 * (node->left), and rchild (node->right) are ALL BLACK
		 *
		 * recolor node to restore balance in parent tree, but still
		 * deficient 1 black height -> recurse */
		node->color = RED;

		parent = grandparent;
	}
	/* unwound all the way to root
	 * balance is restored, but black height is still deficient */
	return false;
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
	return rb_replace_black_unwind(root,
				       stack_ptr,
				       grandparent);
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
	bool restored;

	/* can handle trees of upto at least 1,000,000,000 nodes */
	struct RedBlackNode *const restrict replacement_stack[81];
	struct RedBlackNode *const restrict *restrict replacement_stack_ptr;

	lchild = node->left;
	rchild = node->right;

	if (lchild == NULL) {
		*tree = rchild;

		restored = (rchild != NULL);

		if (restored)
			rchild->color = BLACK; /* rchild is RED -> restore */

		return restored;
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
