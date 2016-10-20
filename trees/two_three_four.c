#include "two_three_four.h"

/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define SIZE_NODE_ALLOC_FLOOR						\
(PAGE_SIZE - sizeof(struct TwoThreeFourNodeBlock))
#define COUNT_NODE_ALLOC_FLOOR						\
(SIZE_NODE_ALLOC_FLOOR / sizeof(struct TwoThreeFourNode))
#define COUNT_NODE_ALLOC						\
(COUNT_NODE_ALLOC_FLOOR + 1)
#define SIZE_NODE_ALLOC							\
(COUNT_NODE_ALLOC * sizeof(struct TwoThreeFourNode))
#define SIZE_NODE_BLOCK_ALLOC						\
(sizeof(struct TwoThreeFourNodeBlock) + SIZE_NODE_ALLOC)

/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
/* node block operations */
static inline void
free_node_blocks(struct TwoThreeFourNodeBlock *restrict block)
{
	if (block == NULL)
		return;

	struct TwoThreeFourNodeBlock *restrict next;

	while (1) {
		next = block->link;

		free(block);

		if (next == NULL)
			return;

		block = next;
	}
}

static inline void
push_new_node_block(struct TwoThreeFourNodeBlock *restrict *const restrict head)
{

	struct TwoThreeFourNodeBlock *const restrict block
	= malloc(SIZE_NODE_BLOCK_ALLOC);

	if (UNLIKELY(block == NULL)) {
		free_node_blocks(*head);
		EXIT_ON_FAILURE("failed to malloc %zu bytes for 'block'",
				SIZE_NODE_BLOCK_ALLOC);
	}

	struct TwoThreeFourNode *const restrict node_alloc
	= (struct TwoThreeFourNode *) (block + 1l);

	block->base    = node_alloc;
	block->current = node_alloc;
	block->until   = node_alloc + COUNT_NODE_ALLOC;
	block->link    = *head;

	*head = block;
}

static inline struct TwoThreeFourNode *
node_block_next(struct TwoThreeFourNodeBlock *restrict *const restrict head)
{
	struct TwoThreeFourNode *restrict node;

	struct TwoThreeFourNodeBlock *restrict block;

	block = *head;
	node  = block->current;

	if (node == block->until) {
		push_new_node_block(head);
		block = *head;
		node = block->current;
	}

	++(block->current);

	return node;
}

/* alloc operations */
static inline void
alloc_init(struct TwoThreeFourAlloc *const restrict alloc)
{
	alloc->free   = NULL;
	alloc->active = NULL;

	push_new_node_block(&alloc->active);
}

static inline struct TwoThreeFourNode *
alloc_pop(struct TwoThreeFourAlloc *const restrict alloc)
{
	struct TwoThreeFourNode *restrict node;

	node = alloc->free;

	if (node == NULL)
		node = node_block_next(&alloc->active);
	else
		alloc->free = node->link;

	return node;
}

static inline void
alloc_free(struct TwoThreeFourAlloc *const restrict alloc)
{
	struct TwoThreeFourNodeBlock *const restrict active
	= alloc->active;

	free_node_blocks(active->link);

	free(active);
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
