#include "two_three_four.h"


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
two_three_four_alloc_init(struct TwoThreeFourAlloc *const restrict alloc)
{
	alloc->base = malloc(sizeof(struct TwoThreeFourNode));

	if (UNLIKELY(alloc->base == NULL))
		EXIT_ON_FAILURE("failed to malloc %zu bytes for tree",
				sizeof(struct TwoThreeFourNode));

	alloc->current = alloc->base;
	alloc->until   = alloc->current + 1l;
	alloc->size    = sizeof(struct TwoThreeFourNode);
}

static inline void
two_three_four_alloc_expand(struct TwoThreeFourAlloc *const restrict alloc)
{
	alloc->size *= 2;

	struct TwoThreeFourNode *const restrict next_base
	= realloc(alloc->base,
		  alloc->size);

	if (next_base != alloc->base) {
		if (UNLIKELY(next_base == NULL)) {
			free(alloc->base);
			EXIT_ON_FAILURE("failed to expand alloc to %zu bytes",
					alloc->size);
		} else {
			alloc->current += (next_base - alloc->base);
			alloc->base = next_base;
		}
	}

	alloc->until = next_base + alloc->size;
}

static inline struct TwoThreeFourNode *
two_three_four_alloc_pop(struct TwoThreeFourAlloc *const restrict alloc)
{
	if (alloc->current == alloc->until)
		two_three_four_alloc_expand(alloc);

	struct TwoThreeFourNode *const restrict node = alloc->current;

	++(alloc->current);

	return node;
}


/* API
 * ────────────────────────────────────────────────────────────────────────── */
void
two_three_four_tree_init(struct TwoThreeFourTree *const restrict tree,
			 Comparator *const compare,
			 Stringifier *const stringify_key,
			 Stringifier *const stringify_value)
{
	tree->root	      = NULL;
	tree->compare	      = compare;
	tree->stringify_key   = stringify_key;
	tree->stringify_value = stringify_value;

	two_three_four_alloc_init(&tree->alloc);
}

extern inline void
two_three_four_tree_free(struct TwoThreeFourTree *const restrict tree);
