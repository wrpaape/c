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

	} while (update_hash(&hash_string,
			     lead_coeff,
			     &string,
			     &string_next));
}

int
main(int argc,
     char *argv[])
{
	if (argc == 3)
		print_matches((const unsigned char *) argv[1],
			      (const unsigned char *) argv[2]);
	else
		WRITE_OUTPUT_LITERAL("substrings <substring> <string>\n");

	return 0;
}
