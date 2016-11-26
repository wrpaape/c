#include "key_utils.h"

extern inline void
exit_on_failure(const char *const restrict failure,
		const size_t length_failure)
__attribute__((noreturn));

extern inline void
exit_on_success(const char *const restrict message,
		const size_t length_message)
__attribute__((noreturn));



static inline void
get_string(unsigned char *restrict *const restrict string,
	   size_t *const restrict length)
{
	static unsigned char buffer[2048];
	static size_t rem_size		     = sizeof(buffer);
	static unsigned char *restrict alloc = &buffer[0];

	const ssize_t size_read = read(STDIN_FILENO,
				       alloc,
				       rem_size);

	if (size_read >= 0) {
		if (size_read < (ssize_t) rem_size) {
			*string = alloc;
			*length = (size_t) size_read;

			const size_t size_string = size_read + 1;

			alloc	 += size_string;
			rem_size -= size_string;
			return;

		} else {
			EXIT_ON_FAILURE("get_string failure: out of space");
		}
	} else {
		EXIT_ON_FAILURE("get_string failure: read");
	}
	__builtin_unreachable();
}

/* taken from bob jenkin's website */
/* The mixing step */
#define mix64(a, b, c)							    \
{									    \
	a -= b;  a -= c;  a ^= (c >> 43); b -= c;  b -= a;  b ^= (a <<  9); \
	c -= a;  c -= b;  c ^= (b >>  8); a -= b;  a -= c;  a ^= (c >> 38); \
	b -= c;  b -= a;  b ^= (a << 23); c -= a;  c -= b;  c ^= (b >>  5); \
	a -= b;  a -= c;  a ^= (c >> 35); b -= c;  b -= a;  b ^= (a << 49); \
	c -= a;  c -= b;  c ^= (b >> 11); a -= b;  a -= c;  a ^= (c >> 12); \
	b -= c;  b -= a;  b ^= (a << 18); c -= a;  c -= b;  c ^= (b >> 22); \
}

static inline uint64_t
jhash64(register unsigned char *k,    /* the key */
	register const size_t length, /* num bytes of key */
	register uint64_t level)      /* prev hash or arb val */
{
	register uint64_t a, b, c; /* the internal state */
	register size_t len;	   /* how many key bytes still need mixing */

	/* Set up the internal state */
	a = b = level;		  /* variable initialization of internal state */
	c = 0x9e3779b97f4a7c13LL; /* the golden ratio; an arbitrary value */

	/*---------------------------------------- handle most of the key */
	for (len = length; len >= 24; k += 24, len -= 24) {

		a += (  k[ 0]			 + ((uint64_t) k[ 1] <<  8)
		      + ((uint64_t) k[ 2] << 16) + ((uint64_t) k[ 3] << 24)
		      + ((uint64_t) k[ 4] << 32) + ((uint64_t) k[ 5] << 40)
		      + ((uint64_t) k[ 6] << 48) + ((uint64_t) k[ 7] << 56));

		b += (  k[ 8]			 + ((uint64_t) k[ 9] <<  8)
		      + ((uint64_t) k[10] << 16) + ((uint64_t) k[11] << 24)
		      + ((uint64_t) k[12] << 32) + ((uint64_t) k[13] << 40)
		      + ((uint64_t) k[14] << 48) + ((uint64_t) k[15] << 56));

		c += (  k[16]			 + ((uint64_t) k[17] <<  8)
		      + ((uint64_t) k[18] << 16) + ((uint64_t) k[19] << 24)
		      + ((uint64_t) k[20] << 32) + ((uint64_t) k[21] << 40)
		      + ((uint64_t) k[22] << 48) + ((uint64_t) k[23] << 56));

		mix64(a, b, c);
	}

	/*------------------------------------- handle the last 23 bytes */
	c += length;

	switch (len) {
		/* all the case statements fall through */
		case 23: c += ((uint64_t) k[22] << 56);
		case 22: c += ((uint64_t) k[21] << 48);
		case 21: c += ((uint64_t) k[20] << 40);
		case 20: c += ((uint64_t) k[19] << 32);
		case 19: c += ((uint64_t) k[18] << 24);
		case 18: c += ((uint64_t) k[17] << 16);
		case 17: c += ((uint64_t) k[16] <<  8);
		/* the first byte of c is reserved for the length */
		case 16: b += ((uint64_t) k[15] << 56);
		case 15: b += ((uint64_t) k[14] << 48);
		case 14: b += ((uint64_t) k[13] << 40);
		case 13: b += ((uint64_t) k[12] << 32);
		case 12: b += ((uint64_t) k[11] << 24);
		case 11: b += ((uint64_t) k[10] << 16);
		case 10: b += ((uint64_t) k[ 9] <<  8);
		case  9: b += ((uint64_t) k[ 8]	     );
		case  8: a += ((uint64_t) k[ 7] << 56);
		case  7: a += ((uint64_t) k[ 6] << 48);
		case  6: a += ((uint64_t) k[ 5] << 40);
		case  5: a += ((uint64_t) k[ 4] << 32);
		case  4: a += ((uint64_t) k[ 3] << 24);
		case  3: a += ((uint64_t) k[ 2] << 16);
		case  2: a += ((uint64_t) k[ 1] <<  8);
		case  1: a += ((uint64_t) k[ 0]	     );
		/* case 0: nothing left to add */
	}

	mix64(a, b, c);

	/*-------------------------------------------- report the result */
	return c;
}
#undef mix64

static inline uint64_t
hash_string(const struct String *const restrict string)
{
	return jhash64(string->from,
		       string->until - string->from,
		       0xfedcba9876543210LL);
}

struct Key *
get_key(void)
{
	static struct Key buffer[128];
	static struct Key *restrict alloc	      = &buffer[0];
	static struct Key *const restrict alloc_until = &buffer[128];


	if (alloc < alloc_until) {
		struct Key *const restrict key = alloc;
		++alloc;

		get_string(&key->string);

		key->hash = hash_string(&key->string);

		return key;
	}

	EXIT_ON_FAILURE("get_key failure: out of space");
	__builtin_unreachable();
}

static inline int
string_compare(const struct String *const restrict string1,
	       const struct String *const restrict string2)
{
	unsigned char token1;
	unsigned char token2;

	unsigned char *from1;
	unsigned char *from2;

	from1 = string1->from;
	unsigned char *const restrict until1 = string1->until;

	from2 = string2->from;
	unsigned char *const restrict until2 = string2->until;

	while (1) {

	}




}

int
key_compare(const struct Key *const restrict key1,
	    const struct Key *const restrict key2)
{
	return (key1->hash == key2->hash)
	     ? string_compare(&key1->string,
			      &key2->string)
	     : key2->hash - key1->hash;
}


int
main(void)
{
	while (1) {
		get_key();
	}
	return 0;
}
