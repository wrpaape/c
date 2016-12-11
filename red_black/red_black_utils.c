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
red_black_insert_root(struct RedBlackNode *restrict *const restrict tree,
		      const struct Key *const restrict key)
{
	/* static alloc, all pointers NULL */
	struct RedBlackNode *const restrict node = node_alloc_pop();
	node->key   = key;
	node->color = BLACK;
	*tree = node;
}

static inline void
rotate_left(struct RedBlackNode *restrict *const restrict tree,
	    struct RedBlackNode *const restrict parent,
	    struct RedBlackNode *const restrict rchild)
{
	*tree = rchild;

	struct RedBlackNode *const restrict tmp = rchild->left;
	rchild->left  = parent;
	parent->right = tmp;
}

static inline void
rotate_right(struct RedBlackNode *restrict *const restrict tree,
	     struct RedBlackNode *const restrict parent,
	     struct RedBlackNode *const restrict lchild)
{
	*tree = lchild;

	struct RedBlackNode *const restrict tmp = lchild->right;
	lchild->right = parent;
	parent->left  = tmp;
}

void
red_black_insert(struct RedBlackNode *restrict *restrict tree,
		 const struct Key *const restrict key)
{
	struct RedBlackNode *restrict grandparent;
	struct RedBlackNode *restrict parent;
	struct RedBlackNode *restrict node;
	int64_t compare;

	parent = *tree;

	if (parent == NULL) {
		red_black_insert_root(tree,
				      key);
		return;
	}

	compare = key_compare(parent->key,
			      key);

	if (compare == 0)	/* no duplicates */
		return;

	node = (compare < 0)
	     ? parent->left
	     : parent->right;
}

static inline char *
put_indent(char *restrict buffer,
	   const unsigned int level)
{
	char *const restrict indent_until = buffer + level;

	while (buffer < indent_until) {
		*buffer = ' ';
		++buffer;
	}

	return buffer;
}

static inline char *
put_red_black_null(char *restrict buffer,
		   const unsigned int level)
{
}

static inline char *
put_red_black_node(char *restrict buffer,
		   const struct RedBlackNode *const restrict node,
		   const unsigned int level)
{

}

char *
do_red_black_print(const restrict RedBlackNode *const restrict node)
{
}


void
red_black_print(const struct RedBlackNode *const restrict tree)
{
	static char buffer[2048];

}


















