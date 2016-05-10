#include "split_string.h"

#define DEFAULT_SPLIT 'x'
#define DEFAULT_PATTERN "xxoogaxboogaxboo"
#define DEFAULT_LENGTH (16ul * 20ul)

int main(int argc, char *argv[])
{
	char *const raw_string = (argc > 1) ? argv[1]    : set_default_raw();
	const char split_char  = (argc > 2) ? argv[2][0] : DEFAULT_SPLIT;

	printf("raw:\n\t%s\n\nsplit:\n", raw_string);

	char **split_strings = split_raw_string(raw_string,
						split_char);

	while (*split_strings != NULL) {
		printf("\t%s\n", *split_strings);
		++split_strings;
	}

	return 0;
}

char **split_raw_string(char *restrict raw_string,
			const char split_char)
{
	char **split_strings;

	const size_t seg_count = count_segments(raw_string,
						split_char);

	HANDLE_MALLOC(split_strings,
		      sizeof(char *) * (seg_count));

	for (size_t i = 0ul; i < seg_count; ++i) {

		split_strings[i] = raw_string;

		while (*raw_string != split_char)
			++raw_string;

		*raw_string = '\0';
		++raw_string;
	}

	split_strings[seg_count] = NULL;

	return split_strings;
}

inline size_t count_segments(const char *restrict raw_string,
			     const char split_char)
{
	size_t seg_count = 1ul;

	while (*raw_string != '\0') {

		if (*raw_string == split_char)
			++seg_count;

		++raw_string;
	}

	return seg_count;
}

inline char *set_default_raw(void)
{
	char *restrict raw_string;

	HANDLE_MALLOC(raw_string, sizeof(char) * (DEFAULT_LENGTH + 1ul));

	memset_pattern16(raw_string,
			 DEFAULT_PATTERN,
			 sizeof(char) * DEFAULT_LENGTH);

	return raw_string;
}
