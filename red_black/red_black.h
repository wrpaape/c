#ifndef RED_BLACK_H_
#define RED_BLACK_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include "red_black_utils.h"


/* prompts
 * ────────────────────────────────────────────────────────────────────────── */
#define INVALID_INPUT	"invalid input\n"
#define MAIN_PROMPT	"insert (i), find (f), delete (d), print (p), or  quit (q)\n> "
#define INSERT_PROMPT	"input insert string, back (b), print (p), or quit (q)\n> "
#define FIND_PROMPT	"input find string, back (b), print (p), or quit (q)\n> "
#define DELETE_PROMPT	"input delete string, back (b), print (p), or quit (q)\n> "

/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define WRITE_LITERAL(LITERAL)						\
if (write(STDOUT_FILENO,						\
	  LITERAL,							\
	  sizeof(LITERAL) - 1) < 0)					\
	EXIT_ON_FAILURE("write failure")

#define READ_INPUT(BUFFER)						\
read(STDIN_FILENO,							\
     BUFFER,								\
     sizeof(BUFFER) - 1)

#define GET_CHAR(PROMPT,						\
		 BUFFER)						\
WRITE_LITERAL(PROMPT);							\
ssize_t _size_read;							\
do {									\
	_size_read = READ_INPUT(BUFFER);				\
	if (_size_read >= 0) {						\
		if ((_size_read > 0) && (_size_read < 3))		\
			break;						\
		WRITE_LITERAL(INVALID_INPUT);				\
	} else {							\
		EXIT_ON_FAILURE("read failure");			\
	}								\
} while (1)

#endif /* ifndef RED_BLACK_H_ */
