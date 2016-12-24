#include "red_black_delete.h"

static inline bool
rb_replace_black_shallow(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict lnode,
			 struct RedBlackNode *const restrict rnode)
{

	struct RedBlackNode *restrict llchild;
	struct RedBlackNode *restrict lrchild;
	struct RedBlackNode *restrict lrrgrandchild;
	struct RedBlackNode *restrict rrchild; /* rlchild is NULL */
	bool restored;

	if (rnode->color == RED) {
		/* rnode RED, lnode RED -> black height of 1
		 * ────────────────────────────────────────────────────────── */
		rnode->color = BLACK;
		rnode->left  = lnode;

		*tree = rnode;
		return true;
	}

	/* lnode, rnode subtrees have black height of 2
	 * ────────────────────────────────────────────────────────────────── */
	rrchild = rnode->right;

	if (rrchild != NULL) {
		rrchild->color = BLACK; /* must be RED */

		rnode->left = lnode;

		*tree = rnode;
		return true;
	}

	/* rnode is BLACK leaf
	 * ────────────────────────────────────────────────────────────────── */
	lrchild = lnode->right;

	if (lrchild == NULL) {
		lnode->right = rnode; /* lnode is BLACK */

		llchild = lnode->left;

		/* only non-restorable case: lnode and rnode are BLACK leafs
		 * ────────────────────────────────────────────────────────── */
		restored = (llchild != NULL);
		if (restored)
			llchild->color = BLACK;
		else
			rnode->color = RED; /* restore balance */

		*tree = lnode;
		return restored;

	/* RED node exists somewhere to be recolored, always restorable
	 * ────────────────────────────────────────────────────────────────── */
	} else if (lrchild->color == RED) {
		/* lnode is BLACK, lrchild is RED leaf
		 * ────────────────────────────────────────────────────────── */
		lrchild->color = BLACK;
		lrchild->left  = lnode;
		lrchild->right = rnode;

		lnode->right = NULL;

		*tree = lrchild;

	} else {
		/* lnode is RED w/ 2 BLACK children, set BLACK
		 * ────────────────────────────────────────────────────────── */
		lnode->color = BLACK;

		lrrgrandchild = lrchild->right;

		if (lrrgrandchild == NULL) {
			lrchild->right = rnode;
			rnode->color   = RED; /* make node new RED max leaf */

		} else {
			lrchild->right = NULL;

			lrrgrandchild->left = lrchild;
			lrgrandchild->right = rchild;

			lchild->right = lrrgrandchild;
		}

		*tree = lnode;
	}

	return true;
}


/* unwind min successor stack to attempt to restore black height and balance
 * if black height cannot be restored, retore balance and return false */
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


/* attempt to restore black height in the replacement node (lnode) tree (right
 * subtree of deleted BLACK node)
 *
 * if can't, restore balance and return false */
static inline bool
rb_replace_black_rtree(struct RedBlackNode *restrict *restrict root,
		       struct RedBlackNode *const restrict *restrict stack_ptr,
		       struct RedBlackNode *restrict parent,
		       struct RedBlackNode *const restrict lnode,
		       struct RedBlackNode *const restrict lrchild)
{
	struct RedBlackNode *restrict grandparent;
	struct RedBlackNode *restrict rnode;
	struct RedBlackNode *restrict rlchild;
	struct RedBlackNode *restrict rrchild;
	struct RedBlackNode *restrict rllgrandchild;

	if (lrchild != NULL) {
		/* replacement node is BLACK, child is RED leaf -> resture  */
		lrchild->color = BLACK;
		return true;

	} else if (lnode->color == RED) {
		/* no harm no foul, ensure replacement BLACK before return */
		lnode->color = BLACK;
		return true;
	}

	grandparent = *stack_ptr;
	rnode       = parent->right;
	rlchild     = rnode->left;

	if (parent->color == RED) {
		parent->color = BLACK;

		if (grandparent != NULL) /* parent != root */
			root = &grandparent->left;

		/* rnode must be BLACK */
		if (rlchild == NULL) {
			rnode->color = RED;
			rrchild = rnode->right;

			if (rrchild != NULL) {
				/* rrchild must be RED leaf */
				rrchild->color = BLACK;

				*root = rnode;

				rnode->left = parent;

				parent->right = NULL;
			} /* else no need to change root */
		} else {
			*root = rlchild;

			rlchild->left  = parent;
			rlchild->right = rnode;

			parent->right = NULL;

			rnode->left = NULL;
		}

	} else if (rnode->color == RED) {
		 /* can balance AND restore black height */
		parent->right = NULL;

		if (grandparent != NULL) /* parent != root */
			root = &grandparent->left;

		/* rlchild is BLACK, not NULL */
		rllgrandchild = rlchild->left;

		if (rllgrandchild == NULL) {
			rlchild->left = parent; /* put parent on min */

			parent->color = RED;

			*root = rnode;

			rnode->color = BLACK;

		} else {
			rlchild->left = NULL; /* bring ggchild to root */

			*root = rllgrandchild;

			rllgrandchild->color = BLACK;
			rllgrandchild->left  = parent;
			rllgrandchild->right = rnode;
		}

	} else if (rlchild == NULL) {
		rrchild = rnode->right;

		if (rrchild == NULL) {
			rnode->color = RED;
			/* grandparent->parent->rnode balanced, but are
			 * deficient 1 black height -> unwind stack to look for
			 * opportunity to correct
			 * ────────────────────────────────────────────────── */
			return rb_replace_black_unwind(root,
						       stack_ptr,
						       grandparent);
		}

		/* rrchild must be RED leaf */
		rrchild->color = BLACK;

		if (grandparent != NULL) /* parent != root */
			root = &grandparent->left;

		*root = rnode;

		rnode->left = parent;

		parent->right = NULL;

	} else {
		/* granchild is a RED leaf, make new root */
		if (grandparent != NULL) /* parent != root */
			root = &grandparent->left;

		*root = rlchild;

		rlchild->color = BLACK;
		rlchild->left  = parent;
		rlchild->right = rnode;

		parent->right = NULL;

		rnode->left = NULL;
	}

	return true; /* completely restored */
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
			rchild->color = BLACK; /* rchild RED leaf -> restore */

		return restored;

	} else if (rchild == NULL) {
		*tree = lchild;
		lchild->color = BLACK;
		return true; /* lchild must be RED leaf, -> BLACK -> restored */
	}

	/* find min successor, its parent, its right child, and its ancestor
	 * stack of rchild->left->left-> ... ->replacement_parent->replacement
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) /* black height of 1 or 2 */
		return rb_replace_black_shallow(tree,
						lchild,
						rchild);

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

	restored = rb_replace_black_rtree(&rchild,
					  replacement_stack_ptr,
					  replacement_parent,
					  replacement,
					  replacement_child);

	replacement->right = rchild; /* set right subtree of replacement */

	if (restored) {
		/* black height restored through recoloring and/or rotation of
		 * ancestors or right child of replacement
		 *
		 * replacement guaranteed BLACK */
		*tree = replacement;
		replacement->left = lchild;
		return true;
	}

	*tree = lchild;

	/* rchild and replacement are BLACK, right subtree is valid (balanced)
	 * but deficient 1 black height
	 *
	 * lchild has black height of AT LEAST 2 (inclusive)
	 * ────────────────────────────────────────────────────────────────── */

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
