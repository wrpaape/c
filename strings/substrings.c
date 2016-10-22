#include "substrings.h"

#define MERSENNE_PRIME 2305843009213693951

static inline size_t
init_hashes(uint64_t *const restrict lead_coeff_ptr,
	    uint64_t *const restrict hash_substring_ptr,
	    uint64_t *const restrict hash_string_ptr,
	    const unsigned char *restrict substring,
	    const unsigned char *restrict string)
{
	uint64_t hash_substring;
	uint64_t hash_string;
	uint64_t lead_coeff;

	hash_substring = *substring;

	if (hash_substring == 0)
		return 0;

	const unsigned char *const restrict substring_start = substring;

	hash_string = *string;

	if (hash_string == 0)
		return 0;

	lead_coeff = 1;

	while (1) {
		++substring;

		if (*substring == '\0') {
			*lead_coeff_ptr	    = lead_coeff & MERSENNE_PRIME;
			*hash_substring_ptr = hash_substring;
			*hash_string_ptr    = hash_string;
			return substring - substring_start;
		}

		++string;

		if (*string == '\0')
			return 0;

		hash_substring = *substring
			       + ((  (hash_substring & MERSENNE_PRIME)
				   * (UCHAR_MAX & MERSENNE_PRIME))
				  & MERSENNE_PRIME);

		hash_string = *string
			    + ((  (hash_string & MERSENNE_PRIME)
				* (UCHAR_MAX & MERSENNE_PRIME))
			       & MERSENNE_PRIME);

		lead_coeff = (  (lead_coeff & MERSENNE_PRIME)
			      * (UCHAR_MAX & MERSENNE_PRIME))
			   & MERSENNE_PRIME;
	}
}

static inline bool
update_hash(uint64_t *const restrict hash_ptr,
	    const uint64_t lead_coeff,
	    const unsigned char *restrict *const restrict string_ptr,
	    const unsigned char *restrict *const restrict string_next_ptr)
{
	uint64_t hash;

	const unsigned char new = **string_next_ptr;

	const bool status = (new != '\0');

	if (status) {
		++(*string_next_ptr);

		const unsigned char old = **string_ptr;

		++(*string_ptr);

		hash = *hash_ptr;

		hash -= ((  lead_coeff
			  * (old & MERSENNE_PRIME)) & MERSENNE_PRIME);


		hash = (  (hash & MERSENNE_PRIME)
			* (UCHAR_MAX & MERSENNE_PRIME)) & MERSENNE_PRIME;

		*hash_ptr = hash + new;
	}


	return status;
}


static inline void
print_matches(const unsigned char *const restrict substring,
	      const unsigned char *restrict string)
{
	uint64_t hash_substring;
	uint64_t hash_string;
	uint64_t lead_coeff;

	const unsigned char *restrict string_next;

	const size_t length_substring = init_hashes(&lead_coeff,
						    &hash_substring,
						    &hash_string,
						    substring,
						    string);

	if (length_substring == 0)
		return;

	string_next = string + length_substring;

	do {
		if (hash_string == hash_substring) {
			WRITE_OUTPUT(string,
				     length_substring);
			WRITE_OUTPUT_LITERAL("\n");
		}
		/* } else { */
		/* 	printf("string: %.*s (%llu), substring: %.*s (%llu)\n", */
		/* 	       (int) length_substring, string, hash_string, */
		/* 	       (int) length_substring, substring, hash_substring); */
		/* } */

	} while (update_hash(&hash_string,
			     lead_coeff,
			     &string,
			     &string_next));
}

int
fetch_strings(const unsigned char *restrict *const restrict substring_ptr,
	      const unsigned char *restrict *const restrict string_ptr)
{
	int file;
	struct stat stat_buffer;

	file = open("substrings_substring.txt",
		    O_RDONLY);

	if (UNLIKELY(file < 0)) {
		WRITE_OUTPUT_LITERAL("open(substrings_substring.txt) failed\n");
		return 1;
	}

	if (UNLIKELY(fstat(file,
			   &stat_buffer) < 0)) {
		(void) close(file);
		WRITE_OUTPUT_LITERAL("fstat(substring_file) failed\n");
		return 1;
	}


	unsigned char *const restrict substring
	= malloc(stat_buffer.st_size);

	if (UNLIKELY(substring == NULL)) {
		(void) close(file);
		WRITE_OUTPUT_LITERAL("malloc(substring) failed\n");
		return 1;
	}

	if (UNLIKELY(read(file,
			  substring,
			  stat_buffer.st_size) < 0)) {
		free(substring);
		(void) close(file);
		WRITE_OUTPUT_LITERAL("read(substring_file) failed\n");
		return 1;
	}

	if (UNLIKELY(close(file) < 0)) {
		free(substring);
		WRITE_OUTPUT_LITERAL("close(substring_file) failed\n");
		return 1;
	}


	file = open("substrings_string.txt",
		    O_RDONLY);

	if (UNLIKELY(file < 0)) {
		free(substring);
		WRITE_OUTPUT_LITERAL("open(substrings_string.txt) failed\n");
		return 1;
	}

	if (UNLIKELY(fstat(file,
			   &stat_buffer) < 0)) {
		free(substring);
		(void) close(file);
		WRITE_OUTPUT_LITERAL("fstat(string_file) failed\n");
		return 1;
	}

	unsigned char *const restrict string
	= malloc(stat_buffer.st_size);

	if (UNLIKELY(string == NULL)) {
		free(substring);
		(void) close(file);
		WRITE_OUTPUT_LITERAL("malloc(string) failed\n");
		return 1;
	}

	if (UNLIKELY(read(file,
			  string,
			  stat_buffer.st_size) < 0)) {
		free(substring);
		free(string);
		(void) close(file);
		WRITE_OUTPUT_LITERAL("read(string_file) failed\n");
		return 1;
	}

	if (UNLIKELY(close(file) < 0)) {
		free(substring);
		free(string);
		WRITE_OUTPUT_LITERAL("close(string_file) failed\n");
		return 1;
	}

	*substring_ptr = substring;
	*string_ptr    = string;
	return 0;
}

int
main(void)
{
	const unsigned char *substring;
	const unsigned char *string;

	const int exit_status = fetch_strings(&substring,
					      &string);

	if (exit_status == 0) {
		print_matches(substring,
			      string);

		free((void *) substring);
		free((void *) string);
	}

	return exit_status;
}
