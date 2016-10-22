#include <stdlib.h>	/* malloc, free */
#include <stdio.h>	/* puts */
#include <stdbool.h>	/* bool */
#include <limits.h>	/* UCHAR_MAX */


struct Segment {
	const char *restrict from;
	const char *restrict until;
};

static inline bool
is_space(const char token)
{
	static const bool space_map[UCHAR_MAX + 1] = {
		[' ']  = true,
		['\f'] = true,
		['\n'] = true,
		['\r'] = true,
		['\t'] = true,
		['\v'] = true
	};

	return space_map[(unsigned char) token];
}

static inline void
scan_words(size_t *const restrict size_words,
	   size_t *const restrict count_segments,
	   const char *const restrict words)
{
	size_t count;
	bool is_segment_space;
	const char *restrict ptr;
	char token;

	token = *words;

	if (token == '\0') {
		*size_words	= 1;
		*count_segments = 0;
		return;
	}

	is_segment_space = is_space(token);
	ptr		 = words;
	count		 = 1;

	while (1) {
		while (1) {
			++ptr;
			token = *ptr;

			if (token == '\0') {
				*size_words	= ptr + 1l - words;
				*count_segments = count;
				return;
			}

			if (is_space(token) != is_segment_space)
				break;
		}

		is_segment_space = !is_segment_space;

		++count;
	}
}

static inline void
populate_segments(struct Segment *restrict segment,
		  const size_t count_segments,
		  const char *restrict words)
{
	bool is_segment_space;

	struct Segment *const restrict last_segment
	= segment + count_segments - 1l;

	is_segment_space = is_space(*words);

	while (1) {
		segment->from = words;

		do {
			++words;
		} while (is_space(*words) == is_segment_space);

		segment->until = words;
		++segment;

		if (segment == last_segment)
			break;

		is_segment_space = !is_segment_space;
	}

	segment->from = words;

	do {
		++words;
	} while (*words != '\0');

	segment->until = words;
}

static inline char *
put_segment(char *restrict buffer,
	    const struct Segment *const restrict segment)
{
	const char *restrict from;

	from = segment->from;

	const char *const restrict until = segment->until;

	do {
		*buffer = *from;
		++buffer;
		++from;
	} while (from < until);

	return buffer;
}

static inline void
populate_buffer(char *restrict buffer,
		const struct Segment *const restrict segments,
		const size_t count_segments)
{
	const struct Segment *restrict segment;

	segment = segments + count_segments - 1l;

	do {
		buffer = put_segment(buffer,
				     segment);

		--segment;
	} while (segment >= segments);

	*buffer = '\0';
}



static inline void
reverse_words(const char *const restrict words)
{
	size_t size_words;
	size_t count_segments;

	scan_words(&size_words,
		   &count_segments,
		   words);

	if (count_segments < 2)
		return;

	char *const restrict buffer
	= malloc(size_words + (sizeof(struct Segment) * count_segments));

	if (buffer == NULL) {
		puts("malloc failure");
		return;
	}

	struct Segment *const restrict segments
	= (struct Segment *const restrict) (buffer + size_words);

	populate_segments(segments,
			  count_segments,
			  words);

	populate_buffer(buffer,
			segments,
			count_segments);

	puts(buffer);

	free(buffer);
}

int
main(int argc,
     char *argv[])
{
	if (argc == 2)
		reverse_words(argv[1]);
	else
		puts("reverse_words <words>");
}
