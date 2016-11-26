#ifndef RED_BLACK_UTILS_H_
#define RED_BLACK_UTILS_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <unistd.h>	/* read, write */
#include <stdlib.h>	/* exit */
#include <stdint.h>	/* u|int32|64_t */

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define EXIT_ON_FAILURE(FAILURE)					\
exit_on_failure(FAILURE "\n",						\
		sizeof(FAILURE))

#define EXIT_ON_SUCCESS(SUCCESS)					\
exit_on_success(SUCCESS "\n",						\
		sizeof(SUCCESS))


/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Key {
	int64_t hash;
	const unsigned char *string;
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

inline void
exit_on_success(const char *const restrict message,
		const size_t length_message)
{
	(void) write(STDOUT_FILENO,
		     message,
		     length_message);
	exit(EXIT_SUCCESS);
	__builtin_unreachable();
}

struct Key *
get_key(void);

int64_t
key_compare(const struct Key *const restrict key1,
	    const struct Key *const restrict key2);


#endif /* ifndef RED_BLACK_UTILS_H_ */
