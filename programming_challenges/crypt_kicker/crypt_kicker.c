#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol, exit */
#include <stdio.h>	/* fgets */
#include <limits.h>	/* UCHAR_MAX */
#include <stddef.h>	/* uint64_t */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define LINE_CHAR_COUNT_MAX	80
#define LINE_SIZE_MAX		(LINE_CHAR_COUNT_MAX + 2) /* \n \0 */
#define WORD_LENGTH_MAX		16
#define MAP_COUNT_MAX		1000

#ifndef UINT64_MAX
#	error uint64_t not supported
#endif	/* ifdef UINT64_MAX */


/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct String {
	const char *restrict from;
	const char *restrict until;
};

struct Word {
	char buffer[WORD_LENGTH_MAX];
	const char *restrict until;
};

struct MapEntry {
	struct Word word;
	struct MapEntry *peer;
};

struct MapNode {
	uint64_t key;
	struct MapEntry *matches;
	struct MapNode *next;
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
#define NORMAL_MAP_LENGTH 1024
#define NORMAL_MAP_INDEX(KEY) (KEY & 1023)

static struct MapNode *normal_map[NORMAL_MAP_LENGTH];




/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define READ_INTEGER(STRING)						\
(int) strtol(STRING,							\
	     &STRING,							\
	     10)

static inline void
exit_on_failure(const char *const restrict failure)
__attribute__((noreturn));

static inline void
exit_on_failure(const char *const restrict failure)
{
	perror(failure);
	exit(1);
}


/* hash string of upto 16 chars long according to normalized sequence */
static inline uint64_t
map_key_create(const char *restrict from,
	       const char *const restrict until)
{
	int *restrict normal;
	int unique_count;
	unsigned int shift;
	uint64_t key;

	int normal_set[26] = {
		[0 ... 25] = -1
	};

	normal_set[*from - 'a'] = 0;

	unique_count = 1;
	shift = 0;
	key = 0;

	while (1) {
		++from;

		if (from == until)
			return key;

		normal = &normal_set[*from - 'a'];

		if (*normal < 0) {
			*normal = unique_count;
			++unique_count;
		}

		key |= (((uint64_t) *normal) << shift);

		shift += 4;
	}
}

static inline void
word_init(struct Word *const restrict word,
	  const char *restrict line,
	  const char *const restrict until)
{
	char *restrict word_from;

	word_from = &word->buffer[0];

	do {
		*word_from = *from;
		++word_from;
		++from;
	} while (from < until);

	word->until = word_from;
}

static inline void
normal_map_insert(const char *restrict from,
		  const char *restrict until)
{
	static struct MapEntry map_entries[MAP_COUNT_MAX];
	static struct MapEntry *restrict map_entry_alloc = &map_entries[0];

	static struct MapNode map_nodes[MAP_COUNT_MAX];
	static struct MapNode *restrict map_node_alloc = &map_nodes[0];


	struct MapNode *restrict node;
	struct MapNode *restrict *restrict node_ptr;

	struct MapEntry *const restrict entry = map_entry_alloc;
	++map_entry_alloc;

	word_init(&entry->word,
		  from,
		  until);

	const uint64_t key = map_key_create(from,
					    until);

	node_ptr = &normal_map[NORMAL_MAP_INDEX(key)];

	while (1) {
		node = *node_ptr;

		/* new top-level node */
		if (node == NULL) {
			node = map_node_alloc;
			++map_node_alloc;

			*node_ptr = node;

			node->key     = key;
			node->matches = entry;
			return;
		}

		/* push into list of matches */
		if (node->key == key) {
			entry->peer   = node->matches;
			node->matches = entry;
			return;
		}

		/* hash collision */
		node_ptr = &node->next;
	}
}


static inline const struct MapEntry *
normal_map_find(const char *restrict from,
		const char *restrict until)
{

	const uint64_t key = map_key_create(from,
					    until);

	struct MapNode *restrict node;

	node = normal_map[NORMAL_MAP_INDEX(key)];

	while (1) {
		if (node == NULL)
			return NULL;

		if (node->key == key)
			return node->matches;

		node = node->next;
	}
}



static inline void
cipher_copy(char *const restrict cipher1,
	    const char *const restrict cipher2)
{
	struct CipherBuffer {
		char _bytes[26];
	};

	*((struct CipherBuffer *const restrict) cipher1)
	= *((struct CipherBuffer *const restrict) cipher2);
}

static inline void
cipher_init(char *const restrict cipher)
{
	static const char unset[26];

	cipher_copy(cipher,
		    &unset[0]);
}


static inline bool
cipher_update(char *const restrict cipher,
	      const struct Word *const restrict word,
	      const char *restrict encrypted)
{
	const char *restrict word_from	      = &word->buffer[0];
	const char *const restrict word_until = word->until;

	char *restrict cipher_ptr;

	while (1) {
		cipher_ptr = &cipher[*word_from - 'a'];

		if (*cipher_ptr == '\0')
			*cipher_ptr = *encrypted;
		else if (*cipher_ptr != *encrypted)
			return false;


		++word_from;

		if (word_from == word_until)
			return true;

		++encrypted;
	}
}


static inline void
print_asterisks(char *const restrict line)
{
	char *restrict ptr;

	ptr = line;

	while (1) {
		switch (*ptr) {
		case ' ':
			break;

		case '\n':
			++ptr;

			if (write(STDOUT_FILENO,
				  line,
				  ptr - line) < 0)
				exit_on_failure("write");

			return;

		default:
			*ptr = '*';
		}

		++ptr;
	}
}




/* int */
/* main(void) */
/* { */
/* 	char *test; */
/* 	uint64_t key; */

/* 	test = "stuff"; */

/* 	key = map_key_create(test, */
/* 			     test + 5); */

/* 	printf("key: %llX (%llu)\n", key, key); */

/* 	test = "grass"; */

/* 	key = map_key_create(test, */
/* 			     test + 5); */

/* 	printf("key: %llX (%llu)\n", key, key); */

/* 	print_asterisks(&string[0]); */

/* 	return 0; */
/* } */



int
main(void)
{
	char line[LINE_SIZE_MAX];
	long rem_words;

	if (fgets(&line[0],
		  LINE_SIZE_MAX,
		  stdin) == NULL)
		exit_on_failure("fgets");


	rem_words = strtol(&line[0],
			   NULL)


	return 0;
}
