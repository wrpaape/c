#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol */
#include <stdio.h>	/* getline */
#include <limits.h>	/* UCHAR_MAX */
#include <stddef.h>	/* uint64_t */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define LINE_LENGTH_MAX	80
#define WORD_LENGTH_MAX	16
#define DICT_COUNT_MAX	1000

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

struct DictNode {
	uint64_t key;
	struct Word value;
};


/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define READ_INTEGER(STRING)						\
(int) strtol(STRING,							\
	     &STRING,							\
	     10)


static inline uint64_t
dict_key_create(const char *restrict from,
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

		printf("normal: %d (%c), shift: %u, shifted: %#017llX\n",
		       *normal, *from, shift, ((uint64_t) *normal) << shift);

		key |= (((uint64_t) *normal) << shift);

		shift += 4;
	}
}


static inline struct DictNode *
pop_dict_node(void)
{
	static struct DictNode nodes[DICT_COUNT_MAX];
	static struct DictNode *restrict head = &nodes[0];

	struct DictNode *const restrict node = head;

	++head;

	return node;
}



static inline void
cipher_init(char *const restrict cipher)
{
	struct CipherBuffer {
		char _bytes[26];
	};

	static const struct CipherBuffer unset = {
		._bytes = { '\0' }
	};

	*((struct CipherBuffer *const restrict) cipher) = unset;
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

int
main(void)
{
	char *test = "abcdefghijklmnop";
	char *until = test + 16;

	uint64_t key = dict_key_create(test,
				       until);

	printf("key: %llX (%llu)\n", key, key);

	return 0;
}



/* int */
/* main(void) */
/* { */
/* 	char *line; */
/* 	size_t capacity; */

/* 	line	 = NULL; */
/* 	capacity = 0; */

/* 	while (getline(&line, */
/* 		       &capacity, */
/* 		       stdin) > 0) */
/* 		puts(is_jolly_jumper(line) ? "Jolly" : "Not jolly"); */

/* 	free(line); */
/* 	return 0; */
/* } */
