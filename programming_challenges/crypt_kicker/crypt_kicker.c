#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol, exit */
#include <stdio.h>	/* fgets */
#include <limits.h>	/* UCHAR_MAX */
#include <stddef.h>	/* uint64_t */
#include <errno.h>	/* errno */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define LINE_CHAR_COUNT_MAX	80
#define LINE_SIZE_MAX		(LINE_CHAR_COUNT_MAX + 2) /* \n \0 */
#define WORD_LENGTH_MAX		16
#define WORD_COUNT_MAX		1000

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
	__builtin_unreachable();
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
	  const char *restrict from,
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
	static struct MapEntry map_entries[WORD_COUNT_MAX];
	static struct MapEntry *restrict map_entry_alloc = &map_entries[0];

	static struct MapNode map_nodes[WORD_COUNT_MAX];
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
print_buffer(char *const restrict buffer,
	     const size_t size)
{
	if (write(STDOUT_FILENO,
		  buffer,
		  size) >= 0)
		return;

	exit_on_failure("write");
	__builtin_unreachable();
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
			print_buffer(line,
				     ptr - line);

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

static inline bool
fetch_line(char *const restrict line,
	   const int size_max)
{
	if (fgets(line,
		  size_max,
		  stdin) != NULL)
		return true;

	if (feof(stdin))
		return false;

	exit_on_failure("fetch_line");
	__builtin_unreachable();
}


static inline const char *
fetch_line_until(char *restrict line,
		 const int size_max)
{
	if (!fetch_line(line,
			size_max))
		return NULL;


	while (*line != '\n')
		++line;

	return line;
}

static inline int
fetch_word_count(const char *const restrict line)
{
	long word_count;

	word_count = strtol(line,
			    NULL,
			    10);


	if (word_count > 0) {
		if (word_count <= WORD_COUNT_MAX)
			return (int) word_count;

	} else if (word_count == 0) {
		if (errno == 0)
			return 0;
	}

	exit_on_failure("unexpected word_count");
	__builtin_unreachable();
}

const char *
decrypt_next(char *restrict from,
	     char *const restrict base_cipher)
{
	const struct MapEntry *restrict match;
	char *restrict until;
	const char *restrict line_until;
	char next_cipher[26];

	while (1) {
		switch (*from) {
		case ' ':
			++from;
			continue;	/* skip spaces */

		case '\n':
			++from;
			return from;	/* successfully decrypted line */

		default: /* do nothing */;
		}
		break;
	}

	until = from + 1l;

	while (   (*until >= 'a')
	       && (*until <= 'z'))
		++until;

	match = normal_map_find(from,
				until);

	while (1) {
		if (match == NULL)
			return NULL;


		cipher_copy(&next_cipher[0],
			    base_cipher);

		if (cipher_update(&next_cipher[0],
				  &match->word,
				  from)) {
			line_until = decrypt_next(until,
						  &next_cipher[0]);

			if (line_until != NULL)
				return line_until;
		}

		match = match->peer;
	}
}

static inline void
decrypt_line(char *const restrict line)
{
	char cipher[26];

	const char *const restrict until = decrypt_next(line,
							&cipher[0]);

	if (until == NULL)
		print_asterisks(line);
	else
		print_buffer(line,
			     until - line);
}


int
main(void)
{
	char line[LINE_SIZE_MAX];
	const char *restrict until;
	int rem_words;

	if (!fetch_line(&line[0],
			LINE_SIZE_MAX))
		exit_on_failure("no word_count");

	rem_words = fetch_word_count(line);

	while (rem_words > 0) {
		--rem_words;

		until = fetch_line_until(&line[0],
					 LINE_SIZE_MAX);

		if (until == NULL)
			exit_on_failure("expected more words");

		if (until != &line[0])
			normal_map_insert(&line[0],
					  until);
	}

	while (fetch_line(&line[0],
			  LINE_SIZE_MAX))
			decrypt_line(&line[0]);

	return 0;
}
