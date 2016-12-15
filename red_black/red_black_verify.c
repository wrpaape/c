#include "red_black_verify.h"

int
do_red_black_verify(const struct RedBlackNode *const restrict node,
		    const struct Key *const restrict min,
		    const struct Key *const restrict max,
		    const enum Color parent_color,
		    int black_height)
{
	if (node == NULL)
		return black_height + 1;

	const struct Key *const restrict node_key = node->key;

	if (   (key_compare(node_key,
			    min) < 0)
	    || (key_compare(node_key,
			    max) > 0))
		return -1;

	const enum Color node_color = node->color;

	if (node_color == RED) {
		if (parent_color == RED)
			return -1;

	} else {
		++black_height;
	}

	const int left_black_height = do_red_black_verify(node->left,
							  min,
							  node_key,
							  node_color,
							  black_height);

	if (left_black_height < 0)
		return -1;

	const int right_black_height = do_red_black_verify(node->right,
							   node_key,
							   max,
							   node_color,
							   black_height);

	return (right_black_height == left_black_height)
	     ? right_black_height
	     : -1;
}

bool
red_black_verify(const struct RedBlackNode *const restrict tree)
{
	return (do_red_black_verify(tree,
				    &KEY_MIN,
				    &KEY_MAX,
				    RED,
				    0) >= 0);
}
