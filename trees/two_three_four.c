#include "two_three_four.h"


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
/* alloc operations */
static inline void
alloc_init(struct TwoThreeFourAlloc *const restrict alloc)
{
	alloc->base = malloc(sizeof(struct TwoThreeFourNode));

	if (UNLIKELY(alloc->base == NULL))
		EXIT_ON_FAILURE("failed to malloc %zu bytes for 'alloc'",
				sizeof(struct TwoThreeFourNode));

	alloc->current = alloc->base;
	alloc->until   = alloc->current + 1l;
	alloc->size    = sizeof(struct TwoThreeFourNode);
}

static inline void
alloc_expand(struct TwoThreeFourAlloc *const restrict alloc)
{
	alloc->size *= 2;

	struct TwoThreeFourNode *const restrict next_base
	= realloc(alloc->base,
		  alloc->size);

	if (next_base != alloc->base) {
		if (UNLIKELY(next_base == NULL)) {
			free(alloc->base);
			EXIT_ON_FAILURE("failed to expand 'alloc' to %zu bytes",
					alloc->size);
		} else {
			alloc->current += (next_base - alloc->base);
			alloc->base = next_base;
		}
	}

	alloc->until = next_base + alloc->size;
}

static inline struct TwoThreeFourNode *
alloc_pop(struct TwoThreeFourAlloc *const restrict alloc)
{
	if (alloc->current == alloc->until)
		alloc_expand(alloc);

	struct TwoThreeFourNode *const restrict node = alloc->current;

	++(alloc->current);

	return node;
}

static inline void
alloc_free(struct TwoThreeFourAlloc *const restrict alloc)
{
	free(alloc->base);
}

/* tuple operations */
static inline void
tuple_set(struct TwoThreeFourTuple *const restrict tuple,
	  void *key,
	  void *value)
{
	tuple->key   = key;
	tuple->value = value;
}

/* cache operations */
static inline void
cache_init(struct TwoThreeFourCache *const restrict cache,
	   Comparator *const compare)
{
	cache->compare = compare;
	alloc_init(&cache->alloc);
}

/* stringifier operations */
static inline void
stringifer_init(struct TwoThreeFourStringifier *const restrict stringifier,
		StringPutter *const put_key,
		StringPutter *const put_value)
{
	stringifier->put_key   = put_key;
	stringifier->put_value = put_value;
}

static inline char *
stringifer_put_tuple(struct TwoThreeFourStringifier *const restrict stringifer,
		     struct TwoThreeFourTuple *const restrict tuple,
		     char *restrict buffer)
{
	*buffer = '{';
	++buffer;
	*buffer = ' ';

	buffer = stringifer->put_key(buffer + 1l,
				     tuple->key);
	*buffer = ':';
	++buffer;
	*buffer = ' ';

	buffer = stringifer->put_value(buffer + 1l,
				       tuple->value);

	*buffer = ' ';
	++buffer;
	*buffer = '}';

	return buffer + 1l;
}


/* node operations */
static inline void
node_children_init(struct TwoThreeFourNode *restrict *const restrict children)
{
	struct TwoThreeFourChildrenBuffer {
		struct TwoThreeFourNode *children[4];
	};

	static const struct TwoThreeFourChildrenBuffer null_children = {
		.children = { NULL, NULL, NULL, NULL }
	};

	*((struct TwoThreeFourChildrenBuffer *) (children)) = null_children;
}

static inline void
node_init(struct TwoThreeFourNode *const restrict node,
	  struct TwoThreeFourTuple *const restrict tuple)
{

	node->type	= TTF_TWO_NODE;
	node->tuples[0] = *tuple;

	node_children_init(&node->children[0]);
}

static inline void
node_init_key_value(struct TwoThreeFourNode *const restrict node,
		    void *key,
		    void *value)
{
	node->type = TTF_TWO_NODE;

	tuple_set(&node->tuples[0],
		  key,
		  value);

	node_children_init(&node->children[0]);
}


/* insert helpers */
static inline void
do_insert(struct TwoThreeFourNode *const restrict node,
	  struct TwoThreeFourCache *const restrict cache);

static void
do_insert_two(struct TwoThreeFourNode *const restrict node,
	      struct TwoThreeFourCache *const restrict cache)
{
}

static void
do_insert_three(struct TwoThreeFourNode *const restrict node,
		struct TwoThreeFourCache *const restrict cache)
{
}

static void
do_insert_four(struct TwoThreeFourNode *const restrict node,
	       struct TwoThreeFourCache *const restrict cache)
{
}

static inline void
do_insert(struct TwoThreeFourNode *const restrict node,
	  struct TwoThreeFourCache *const restrict cache)
{
	typedef void
	DoInsert(struct TwoThreeFourNode *const restrict node,
		 struct TwoThreeFourCache *const restrict cache);

	static DoInsert *const do_insert_map[] = {
		[TTF_TWO_NODE]   = &do_insert_two,
		[TTF_THREE_NODE] = &do_insert_three,
		[TTF_FOUR_NODE]  = &do_insert_four
	};

	do_insert_map[node->type](node,
				  cache);
}


/* API
 * ────────────────────────────────────────────────────────────────────────── */
void
two_three_four_tree_init(struct TwoThreeFourTree *const restrict tree,
			 Comparator *const compare,
			 StringPutter *const put_key,
			 StringPutter *const put_value)
{
	tree->root = NULL;

	cache_init(&tree->cache,
		   compare);

	stringifer_init(&tree->stringifier,
			put_key,
			put_value);
}

void
two_three_four_tree_insert(struct TwoThreeFourTree *const restrict tree,
			   void *key,
			   void *value)
{
	if (tree->root == NULL) {
		tree->root = alloc_pop(&tree->cache.alloc);

		node_init_key_value(tree->root,
				    key,
				    value);

	} else {
		tuple_set(&tree->cache.input,
			  key,
			  value);

		do_insert(tree->root,
			  &tree->cache);
	}
}

bool
two_three_four_tree_find(struct TwoThreeFourTree *const restrict tree,
			 void *key,
			 void **const restrict value)
{
	return false;
}

bool
two_three_four_tree_update(struct TwoThreeFourTree *const restrict tree,
			   void *key,
			   void *value)
{
	return false;
}

void
two_three_four_tree_free(struct TwoThreeFourTree *const restrict tree)
{
	alloc_free(&tree->cache.alloc);
}
