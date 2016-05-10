#include <utils/utils.h>

static inline size_t count_segments(const char *restrict raw_string,
				    const char split_char);

char **split_raw_string(char *restrict raw_string,
			const char split_char);

static inline char *set_default_raw(void);
