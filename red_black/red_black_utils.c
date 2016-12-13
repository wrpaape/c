#include "red_black_utils.h"

static inline struct RedBlackNode *
node_alloc_pop(void)
{
	static struct RedBlackNode buffer[128];
	static struct RedBlackNode *restrict alloc	       = &buffer[0];
	static struct RedBlackNode *const restrict alloc_until = &buffer[128];

	if (alloc < alloc_until) {
		struct RedBlackNode *const restrict node = alloc;
		++alloc;

		return node;
	}

	EXIT_ON_FAILURE("node_alloc_pop failure: out of space");
	__builtin_unreachable();
}


bool
red_black_find(const struct RedBlackNode *restrict node,
	       const struct Key *const restrict key)
{
	int64_t compare;

	while (node != NULL) {
		compare = key_compare(node->key,
				      key);

		if (compare < 0)
			node = node->left;
		else if (compare > 0)
			node = node->right;
		else
			return true;
	}

	return false;
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

static inline struct RedBlackNode *
rb_new_node(const struct Key *const restrict key)
{
	/* static alloc, all pointers == NULL, color == RED */
	struct RedBlackNode *const restrict node = node_alloc_pop();
	node->key = key;

	return node;
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
red_black_insert(struct RedBlackNode *restrict *restrict tree,
		 const struct Key *const restrict key)
{
	int64_t compare;
	struct RedBlackNode *restrict parent;

	struct RedBlackNode *const restrict grandparent = *tree;

	if (grandparent == NULL) {
		rb_insert_root(tree,
			       key);

	} else {
		compare = key_compare(grandparent->key,
				      key);

		if (compare < 0) {
			parent = grandparent->left;

			if (parent == NULL) {
				grandparent->left = rb_new_node(key);

			} else {
				compare = key_compare(parent->key,
						      key);

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
				compare = key_compare(parent->key,
						      key);

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

		rb_rotate_left(&grandparent->left,
			       parent,
			       parent->right);

		rb_rotate_right(tree,
				grandparent,
				parent);
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

		rb_rotate_right(&grandparent->right,
				parent,
				parent->left);

		rb_rotate_left(tree,
			       grandparent,
			       parent);
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

	const int64_t compare = key_compare(next->key,
					    key);

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
		parent->right = rb_new_node(key);
		return rb_insert_correct_lr(tree,
					    grandparent,
					    parent);
	}

	const int64_t compare = key_compare(next->key,
					    key);

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
						    parent);
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

	const int64_t compare = key_compare(next->key,
					    key);

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
		parent->left = rb_new_node(key);
		return rb_insert_correct_rl(tree,
					    grandparent,
					    parent);
	}

	const int64_t compare = key_compare(next->key,
					    key);

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
						    parent);
		}
	}

	return CORRECT_DONE;
}

static inline char *
put_indent(char *restrict buffer,
	   const unsigned int level)
{
	char *const restrict indent_until = buffer + (2 * level);

	while (buffer < indent_until) {
		*buffer = '\t';
		++buffer;
		*buffer = '\t';
		++buffer;
	}

	return buffer;
}

static inline char *
put_red_black_null(char *restrict buffer,
		   const unsigned int level)
{
	buffer = put_indent(buffer,
			    level);

	*buffer = '('; ++buffer;
	*buffer = 'B'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = 'A'; ++buffer;
	*buffer = 'C'; ++buffer;
	*buffer = 'K'; ++buffer;
	*buffer = ')'; ++buffer;
	*buffer = ' '; ++buffer;
	*buffer = 'N'; ++buffer;
	*buffer = 'U'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = '\n';

	return buffer + 1;
}

static inline char *
put_red_black_node(char *restrict buffer,
		   const unsigned int level,
		   const struct RedBlackNode *const restrict node)
{
	buffer = put_indent(buffer,
			    level);

	*buffer = '('; ++buffer;

	if (node->color == RED) {
		*buffer = 'R'; ++buffer;
		*buffer = 'E'; ++buffer;
		*buffer = 'D'; ++buffer;

	} else {
		*buffer = 'B'; ++buffer;
		*buffer = 'L'; ++buffer;
		*buffer = 'A'; ++buffer;
		*buffer = 'C'; ++buffer;
		*buffer = 'K'; ++buffer;
	}

	*buffer = ')'; ++buffer;
	*buffer = ' '; ++buffer;

	buffer = put_key(buffer,
			 node->key);

	*buffer = '\n';

	return buffer + 1;
}

char *
do_red_black_print(char *restrict buffer,
		   const unsigned int level,
		   const struct RedBlackNode *const restrict node)
{
	if (node == NULL)
		return put_red_black_null(buffer,
					  level);

	const unsigned int next_level = level + 1;

	buffer = do_red_black_print(buffer,
				    next_level,
				    node->left);

	buffer = put_red_black_node(buffer,
				    level,
				    node);

	return do_red_black_print(buffer,
				  next_level,
				  node->right);
}


void
red_black_print(const struct RedBlackNode *const restrict tree)
{
	char buffer[4096];

	char *const restrict ptr = do_red_black_print(&buffer[0],
						      0,
						      tree);

	WRITE_STDOUT(&buffer[0],
		     ptr - &buffer[0]);
}


















