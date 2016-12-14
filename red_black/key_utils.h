#ifndef KEY_UTILS_H_
#define KEY_UTILS_H_

#define USE_HASH 0

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <unistd.h>	/* read, write */
#include <stdlib.h>	/* exit */
#include <stdint.h>	/* u|int32|64_t */
#include <string.h>	/* memcpy */
#include <stdio.h>	/* sprintf */

/* helper macros
 * ────────────────────────────────────────────────────────────────────────── */
#define EXIT_ON_FAILURE(FAILURE)					\
exit_on_failure(FAILURE "\n",						\
		sizeof(FAILURE))

#define EXIT_ON_SUCCESS(SUCCESS)					\
exit_on_success(SUCCESS "\n",						\
		sizeof(SUCCESS))

#define WRITE_STDOUT(BUFFER,						\
		     SIZE)						\
if (write(STDOUT_FILENO,						\
	  BUFFER,							\
	  SIZE) < 0)							\
	EXIT_ON_FAILURE("write failure")


/* struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Key {
#if USE_HASH
	int64_t hash;
#endif /* if USE_HASH */
	const unsigned char *string;
};

void
exit_on_failure(const char *const restrict failure,
		const size_t length_failure)
__attribute__((noreturn));

void
exit_on_success(const char *const restrict message,
		const size_t length_message)
__attribute__((noreturn));

struct Key *
pop_key(void);

void
make_key(struct Key *const restrict key,
	 const unsigned char *const restrict string,
	 const size_t length);

void
key_init(struct Key *const restrict key,
	 const unsigned char *const restrict string,
	 const size_t length);

int64_t
key_compare(const struct Key *const restrict key1,
	    const struct Key *const restrict key2);

char *
put_key(char *restrict buffer,
	const struct Key *const restrict key);

#endif /* ifndef KEY_UTILS_H_ */
