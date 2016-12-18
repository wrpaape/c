#include "red_black_delete.h"

static inline void
rb_replace_black_shallow(struct RedBlackNode *restrict *const restrict tree,
			 struct RedBlackNode *const restrict lchild,
			 struct RedBlackNode *const restrict rchild)
{
	struct RedBlackNode *restrict lrchild;

	if (rchild->color == RED) {
		/* rchild RED, lchild RED -> black height of 1 */

		rchild->color = BLACK;
		rchild->left  = lchild; /* lchild must be RED */

		*tree = rchild;

	} else if (lchild->color == RED) {
		/* rchild BLACK, lchild RED -> black height of 2 */

		lchild->color = BLACK;

		*tree = lchild;

		lrchild = lchild->right; /* lrchild must be BLACK */

		lchild->right = rchild;

		rchild->left = lrchild;

		lrchild->color = RED;

	} else {
		/* rchild BLACK, lchild BLACK -> black height of 2 */
		lrchild = lchild->right;

		if (lrchild == NULL) {
			lchild->right = rchild;

			*tree = lchild;

			rchild->color = RED;

		} else {
			lrchild->color = BLACK;
			lrchild->left  = lchild;
			lrchild->right = rchild;

			*tree = lrchild;
		}
	}
}

static inline bool
rb_replace_black(struct RedBlackNode *restrict *const restrict tree,
		 struct RedBlackNode *const restrict node)
{
	struct RedBlackNode *restrict replacement_parent;
	struct RedBlackNode *restrict replacement;
	struct RedBlackNode *restrict next;

	struct RedBlackNode *const restrict lchild = node->left;
	struct RedBlackNode *const restrict rchild = node->right;

	bool black_height_restored;

	if (lchild == NULL) {
		*tree = rchild;

		black_height_restored = (rchild != NULL);

		if (black_height_restored)
			rchild->color = BLACK;

		return black_height_restored;
	}

	if (rchild == NULL) {
		*tree = lchild;
		lchild->color = BLACK;
		return true; /* lchild must be RED, -> BLACK -> restored */
	}

	/* find min successor and its parent
	 * ────────────────────────────────────────────────────────────────── */
	replacement = rchild->left;

	if (replacement == NULL) {
		rb_replace_black_shallow(tree,
					 lchild,
					 rchild);
		return true; /* black height of 1 or 2 restorable in all cases */
	}

	replacement_parent = rchild;

	while (1) {
		next = replacement->left;
		if (next == NULL)
			break;

		replacement_parent = replacement;
		replacement = next;
	}


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
