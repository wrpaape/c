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

	while (1) {
		if (node == NULL)
			return false;

		compare = key_compare(node->key,
				      key);

		if (compare == 0)
			return true;

		node = (compare < 0)
		     ? node->left
		     : node->right;
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
static bool
rb_insert_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);
static bool
rb_insert_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);
static bool
rb_insert_rl(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);
static bool
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key);



void
red_black_insert(struct RedBlackNode *restrict *restrict tree,
		 const struct Key *const restrict key)
{
	uint64_t compare;
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
				node = rb_new_node(key);
				grandparent->left = node;

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
				node = rb_new_node(key);
				grandparent->right = node;

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


static bool
rb_insert_ll(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
	struct RedBlackNode *restrict uncle;

	struct RedBlackNode *const restrict next = parent->left;

	if (next == NULL) {
		parent->left = rb_new_node(key);

		if (parent->color == RED) {
			parent->color = BLACK;

			uncle = grandparent->right;

			if (uncle->color == RED) {
				uncle->color  = BLACK;
				return true; /* need to propogate recolor */
			}

			grandparent->color = RED;

			rb_rotate_right(tree,
					grandparent,
					parent);
		}
	} else {
		const uint64_t compare = key_compare(next,
						     key);

		if (compare < 0) {
			if (rb_insert_ll(&grandparent->parent,
					 parent,
					 next,
					 key)) {
				parent->color = RED;

			}
		} else if (compare > 0) {
			if (rb_insert_lr(&grandparent->parent,
					 parent,
					 next,
					 key)) {

			}
		}
	}

	return false;
}

static bool
rb_insert_lr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
}

static bool
rb_insert_rr(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict grandparent,
	     struct RedBlackNode *const restrict parent,
	     const struct Key *const restrict key)
{
}

static inline char *
put_indent(char *restrict buffer,
	   const unsigned int level)
{
	char *const restrict indent_until = buffer + level;

	while (buffer < indent_until) {
		*buffer = '+';
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

	*buffer = 'N'; ++buffer;
	*buffer = 'U'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = ' '; ++buffer;
	*buffer = '('; ++buffer;
	*buffer = 'B'; ++buffer;
	*buffer = 'L'; ++buffer;
	*buffer = 'A'; ++buffer;
	*buffer = 'C'; ++buffer;
	*buffer = 'K'; ++buffer;
	*buffer = ')'; ++buffer;
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

	buffer = put_key(buffer,
			 node->key);

	*buffer = ' '; ++buffer;
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


















