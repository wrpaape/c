#ifndef RED_BLACK_UTILS_H_
#define RED_BLACK_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <unistd.h>	/* read, write */
#include <stdlib.h>	/* exit */
#include <stdint.h>	/* uint64_t */

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define EXIT_ON_FAILURE(FAILURE)					\
exit_on_failure(FAILURE "\n",						\
		sizeof(FAILURE))


/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct String {
	unsigned char *restrict from;
	unsigned char *restrict until;
};

struct Key {
	uint64_t hash;
	struct String string;
};

inline void
exit_on_failure(const char *const restrict failure,
		const size_t length_failure)
{
	(void) write(STDERR_FILENO,
		     failure,
		     length_failure);
	exit(EXIT_FAILURE);
	__builtin_unreachable();
}

struct Key *
get_key(void);

int
key_compare(const struct Key *const restrict key1,
	    const struct Key *const restrict key2);


#endif /* ifndef RED_BLACK_UTILS_H_ */
