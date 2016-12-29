#ifndef RED_BLACK_JUMP_H_
#define RED_BLACK_JUMP_H_

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <setjmp.h>


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
typedef jmp_buf RedBlackJumpBuffer;


/* jump macros
 * ────────────────────────────────────────────────────────────────────────── */
#define RED_BLACK_SET_JUMP(BUFFER, STATUS)	setjmp(BUFFER)
#define RED_BLACK_LONG_JUMP(BUFFER, STATUS)	longjmp(BUFFER, STATUS)

#endif /* ifndef RED_BLACK_JUMP_H_ */
