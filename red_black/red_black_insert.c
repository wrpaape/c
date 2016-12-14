#include "red_black_insert.h"

static inline struct RedBlackNode *
rb_new_node(const struct Key *const restrict key)
{
	/* static alloc, all pointers == NULL, color == RED */
	static struct RedBlackNode buffer[128];
	static struct RedBlackNode *restrict alloc	       = &buffer[0];
	static struct RedBlackNode *const restrict alloc_until = &buffer[128];

	if (alloc < alloc_until) {
		struct RedBlackNode *const restrict node = alloc;
		++alloc;

		node->key = key;
		return node;
	}

	EXIT_ON_FAILURE("node_alloc_pop failure: out of space");
	__builtin_unreachable();
}

static inline void
rb_insert_root(struct RedBlackNode *restrict *const restrict tree,
	       const struct Key *const restrict key)
{
	struct RedBlackNode *const restrict node = rb_new_node(key);
	node->color = BLACK;
	*tree = node;
}

/* insert state-machine functions */
static enum RedBlackCorrectState
rb_insert_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);
static enum RedBlackCorrectState
rb_insert_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);
static enum RedBlackCorrectState
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);
static enum RedBlackCorrectState
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);

void
red_black_insert(struct RedBlackNode *restrict *const restrict tree,
		 const struct Key *const restrict key)
{
	int64_t compare;
	struct RedBlackNode *restrict parent;

	struct RedBlackNode *const restrict grandparent = *tree;

	if (grandparent == NULL) {
		rb_insert_root(tree,
			       key);

	} else {
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
					(void) rb_insert_ll(tree,
							    grandparent,
							    parent,
							    key);
				else if (compare > 0)
					(void) rb_insert_lr(tree,
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
					(void) rb_insert_rl(tree,
							    grandparent,
							    parent,
							    key);
				else if (compare > 0)
					(void) rb_insert_rr(tree,
							    grandparent,
							    parent,
							    key);
			}
		}
	}
}

static inline void
rb_rotate_left(struct RedBlackNode *restrict *const restrict tree,
	       struct RedBlackNode *const restrict grandparent,
	       struct RedBlackNode *const restrict rparent)
{
	*tree = rparent;

	struct RedBlackNode *const restrict tmp = rparent->left;
	rparent->left      = grandparent;
	grandparent->right = tmp;
}

static inline void
rb_rotate_right(struct RedBlackNode *restrict *const restrict tree,
		struct RedBlackNode *const restrict grandparent,
		struct RedBlackNode *const restrict lparent)
{
	*tree = lparent;

	struct RedBlackNode *const restrict tmp = lparent->right;
	lparent->right    = grandparent;
	grandparent->left = tmp;
}

static inline void
rb_rotate_left_right(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict lparent,
		     struct RedBlackNode *const restrict rchild)
{
	*tree = rchild;

	lparent->right = rchild->left;
	rchild->left   = lparent;

	grandparent->left = rchild->right;
	rchild->right     = grandparent;
}

static inline void
rb_rotate_right_left(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict rparent,
		     struct RedBlackNode *const restrict lchild)
{
	*tree = lchild;

	grandparent->right = lchild->left;
	lchild->left       = grandparent;

	rparent->left = lchild->right;
	lchild->right = rparent;
}

static inline enum RedBlackCorrectState
rb_insert_correct_ll(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent)
{
	if (parent->color == RED) {
		parent->color = BLACK;

		struct RedBlackNode *const restrict uncle = grandparent->right;

		if ((uncle != NULL) && (uncle->color == RED)) {
			uncle->color = BLACK;
			return CORRECT_PREV; /* need to propogate recolor */
		}

		grandparent->color = RED;

		rb_rotate_right(tree,
				grandparent,
				parent);
	}

	return CORRECT_DONE;
}

static inline enum RedBlackCorrectState
rb_insert_correct_lr(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent,
		     struct RedBlackNode *const restrict child)
{
	if (parent->color == RED) {
		struct RedBlackNode *const restrict uncle = grandparent->right;

		if ((uncle != NULL) && (uncle->color == RED)) {
			uncle->color  = BLACK;
			parent->color = BLACK;
			return CORRECT_PREV; /* need to propogate recolor */
		}

		grandparent->color = RED;

		child->color = BLACK;

		rb_rotate_left_right(tree,
				     grandparent,
				     parent,
				     child);
	}

	return CORRECT_DONE;
}

static inline enum RedBlackCorrectState
rb_insert_correct_rr(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent)
{

	if (parent->color == RED) {
		parent->color = BLACK;

		struct RedBlackNode *const restrict uncle = grandparent->left;

		if ((uncle != NULL) && (uncle->color == RED)) {
			uncle->color = BLACK;
			return CORRECT_PREV; /* need to propogate recolor */
		}

		grandparent->color = RED;

		rb_rotate_left(tree,
			       grandparent,
			       parent);
	}

	return CORRECT_DONE;
}

static inline enum RedBlackCorrectState
rb_insert_correct_rl(struct RedBlackNode *restrict *const restrict tree,
		     struct RedBlackNode *const restrict grandparent,
		     struct RedBlackNode *const restrict parent,
		     struct RedBlackNode *const restrict child)
{
	if (parent->color == RED) {
		struct RedBlackNode *const restrict uncle = grandparent->left;

		if ((uncle != NULL) && (uncle->color == RED)) {
			uncle->color  = BLACK;
			parent->color = BLACK;
			return CORRECT_PREV; /* need to propogate recolor */
		}

		grandparent->color = RED;

		child->color = BLACK;

		rb_rotate_right_left(tree,
				     grandparent,
				     parent,
				     child);
	}

	return CORRECT_DONE;
}



static enum RedBlackCorrectState
rb_insert_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
	struct RedBlackNode *const restrict next = parent->left;

	if (next == NULL) {
		parent->left = rb_new_node(key);
		return rb_insert_correct_ll(tree,
					    grandparent,
					    parent);
	}

	const int64_t compare = key_compare(key,
					    next->key);

	if (compare != 0) {
		struct RedBlackNode *restrict *const restrict next_tree
		= &grandparent->left;

		const enum RedBlackCorrectState state
		= (compare < 0)
		? rb_insert_ll(next_tree,
			       parent,
			       next,
			       key)
		: rb_insert_lr(next_tree,
			       parent,
			       next,
			       key);

		if (state == CORRECT_PREV) {
			parent->color = RED;
			return CORRECT_THIS;

		} else if (state == CORRECT_THIS) {
			return rb_insert_correct_ll(tree,
						    grandparent,
						    parent);
		}
	}

	return CORRECT_DONE;
}

static enum RedBlackCorrectState
rb_insert_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
	struct RedBlackNode *const restrict next = parent->right;

	if (next == NULL) {
		struct RedBlackNode *const restrict node = rb_new_node(key);
		parent->right = node;
		return rb_insert_correct_lr(tree,
					    grandparent,
					    parent,
					    node);
	}

	const int64_t compare = key_compare(key,
					    next->key);

	if (compare != 0) {
		struct RedBlackNode *restrict *const restrict next_tree
		= &grandparent->left;

		const enum RedBlackCorrectState state
		= (compare < 0)
		? rb_insert_rl(next_tree,
			       parent,
			       next,
			       key)
		: rb_insert_rr(next_tree,
			       parent,
			       next,
			       key);

		if (state == CORRECT_PREV) {
			parent->color = RED;
			return CORRECT_THIS;

		} else if (state == CORRECT_THIS) {
			return rb_insert_correct_lr(tree,
						    grandparent,
						    parent,
						    parent->right);
		}
	}

	return CORRECT_DONE;
}

static enum RedBlackCorrectState
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
	struct RedBlackNode *const restrict next = parent->right;

	if (next == NULL) {
		parent->right = rb_new_node(key);
		return rb_insert_correct_rr(tree,
					    grandparent,
					    parent);
	}

	const int64_t compare = key_compare(key,
					    next->key);

	if (compare != 0) {
		struct RedBlackNode *restrict *const restrict next_tree
		= &grandparent->right;

		const enum RedBlackCorrectState state
		= (compare < 0)
		? rb_insert_rl(next_tree,
			       parent,
			       next,
			       key)
		: rb_insert_rr(next_tree,
			       parent,
			       next,
			       key);

		if (state == CORRECT_PREV) {
			parent->color = RED;
			return CORRECT_THIS;

		} else if (state == CORRECT_THIS) {
			return rb_insert_correct_rr(tree,
						    grandparent,
						    parent);
		}
	}

	return CORRECT_DONE;
}

static enum RedBlackCorrectState
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
	struct RedBlackNode *const restrict next = parent->left;

	if (next == NULL) {
		struct RedBlackNode *const restrict node = rb_new_node(key);
		parent->left = node;
		return rb_insert_correct_rl(tree,
					    grandparent,
					    parent,
					    node);
	}

	const int64_t compare = key_compare(key,
					    next->key);

	if (compare != 0) {
		struct RedBlackNode *restrict *const restrict next_tree
		= &grandparent->right;

		const enum RedBlackCorrectState state
		= (compare < 0)
		? rb_insert_ll(next_tree,
			       parent,
			       next,
			       key)
		: rb_insert_lr(next_tree,
			       parent,
			       next,
			       key);

		if (state == CORRECT_PREV) {
			parent->color = RED;
			return CORRECT_THIS;

		} else if (state == CORRECT_THIS) {
			return rb_insert_correct_rl(tree,
						    grandparent,
						    parent,
						    parent->left);
		}
	}

	return CORRECT_DONE;
}
