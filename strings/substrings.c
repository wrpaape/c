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
print_matches(const unsigned char *restrict string,
	      const unsigned char *const restrict substring)
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
		/* } else { */
		/* 	printf("string: %.*s (%llu), substring: %.*s (%llu)\n", */
		/* 	       (int) length_substring, string, hash_string, */
		/* 	       (int) length_substring, substring, hash_substring); */
		}

	} while (update_hash(&hash_string,
			     lead_coeff,
			     &string,
			     &string_next));
}

static inline size_t
string_length(const unsigned char *const restrict string)
{
	const unsigned char *restrict ptr = string;

	while (*ptr != '\0')
		++ptr;

	return ptr - string;
}

static inline bool
is_substring(const unsigned char *restrict string,
	     const unsigned char *restrict substring)
{
	while (1) {
		if (*substring == '\0')
			return true;

		if (*string != *substring)
			return false;


		++string;
		++substring;
	}
}

static inline void
print_matches_naive(const unsigned char *restrict string,
		    const unsigned char *const restrict substring)
{

	const size_t length_substring = string_length(substring);

	if (length_substring == 0)
		return;

	while (*string != '\0') {
		if (is_substring(string,
				 substring)) {
			WRITE_OUTPUT(string,
				     length_substring);
			WRITE_OUTPUT_LITERAL("\n");
		}

		++string;
	}
}

/* static inline char * */
/* find_first_match(const unsigned char *const restrict string, */
/* 		 const unsigned char *restrict substring) */
/* { */
/* 	const size_t length_substring = string_length(substring); */
/* } */


int
fetch_strings(const unsigned char *restrict *const restrict string_ptr,
	      const unsigned char *restrict *const restrict substring_ptr)
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

#define KEY "access_token="





static inline int
max(const int x,
    const int y)
{
	return (x > y) ? x : y;
}


const unsigned char *const bm_key = (const unsigned char *) KEY;

#define LENGTH_BM_KEY (sizeof(KEY) - 1)
#define I_MATCH_MAX   (LENGTH_BM_KEY - 1)


static int bad_char_skip[UCHAR_MAX + 1] = {
	[0 ... UCHAR_MAX] = LENGTH_BM_KEY
};

static unsigned int good_suffix_skip[LENGTH_BM_KEY];

static inline void
init_bm_tables(void)
{
	int i;
	unsigned int length_fix;
	unsigned int token;

	length_fix = 0;
	length_max_fix = length_fix;
	i = 0;

	do {
		token = (unsigned int) bm_key[i];

		bad_char_skip[token] = I_MATCH_MAX - i;

		length_fix = (token == bm_key[length_fix])
			   ? (length_fix + 1)
			   : 0;

		length_max_fix[i] = length_fix;

		++i;
	} while (i < length_bm_key);
}


static inline bool
bm_search(const unsigned char *restrict text)
{
	unsigned int i_match;

	unsigned int token;

	const unsigned int length_text = (unsigned int) string_length(text);

	if (length_text < length_bm_key)
		return false;

	const unsigned char *const restrict text_upto
	= text + (length_text - length_bm_key);

	const unsigned int i_match_max = length_bm_key - 1;

	while (1) {
		i_match = i_match_max;

		while (1) {
			token = (unsigned int) text[i_match];

			if (token != bm_key[i_match])
				break;

			if (i_match == 0)
				return true;

			--i_match;
		}


		/* text += max(1, i_match - last_occurrence[token]); */

		if (text > text_upto)
			return false;
	}
}




int
main(void)
{
	const unsigned char *substring;
	const unsigned char *string;

	const int exit_status = fetch_strings(&string,
					      &substring);

	if (exit_status == 0) {
		print_matches(string,
			      substring);
		/* print_matches_naive(string, */
		/* 		    substring); */
		/* print_matches_kmp(string, */
		/* 		  substring); */

		free((void *) string);
		free((void *) substring);
	}

	init_last_occurence_table();

	if (bm_search("ooga booga booooooga looga" KEY "snooga"))
		puts("found bm_key");
	else
		puts("did not find bm_key");



	return exit_status;
}
