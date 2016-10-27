#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */
#include <stdlib.h>	/* strtol */
#include <stdio.h>	/* getline */
#include <limits.h>	/* UCHAR_MAX */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define LINE_LENGTH_MAX	80
#define WORD_LENGTH_MAX	16
#define DICT_COUNT_MAX	1000


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

struct DictKey {
	struct Word normal;
	unsigned int length;
	unsigned int uniques;
};

struct DictNode {
	struct DictKey key;
	struct Word value;
};


/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define READ_INTEGER(STRING)						\
(int) strtol(STRING,							\
	     &STRING,							\
	     10)

#define CLEAR_CIPHER

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
	      const char *const restrict encrypted)
{
	const unsigned char *restrict word_from	       = &word->buffer[0];
	const unsigned char *const restrict word_until = word->until;

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
	char *line;
	size_t capacity;

	line	 = NULL;
	capacity = 0;

	while (getline(&line,
		       &capacity,
		       stdin) > 0)
		puts(is_jolly_jumper(line) ? "Jolly" : "Not jolly");

	free(line);
	return 0;
}
