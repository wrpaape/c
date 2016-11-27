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

	EXIT_ON_FAILURE("get_key failure: out of space");
	__builtin_unreachable();
}

bool
red_black_is_member(const struct RedBlackNode *restrict node,
		    const struct Key *const restrict key)
{
	int64_t compare;

	while (1) {
		if (node == NULL)
			return false;

		compare = key_compare(node->key,
				      key);

		if (compare < 0)
			node = node->less;
		if (compare > 0)
			node = node->more;
		else
			return true;
	}
}
