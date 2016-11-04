#ifndef SUDOKU_SUDOKU_H
#define SUDOKU_SUDOKU_H
/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
/* 9 cell rows + 8 line rows each having 9 segments of width 4 */
#define BOARD_BUFFER_SIZE ((9 + 8) * (9 * 4))

/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Cell {
	unsigned int *restrict row;
	unsigned int *restrict col;
	unsigned int *restrict block;
	char *restrict token;
	struct Cell *restrict next;
};

struct Board {
	struct Cell *restrict rem_cells;
	struct Cell cells[9 * 9];
	unsigned int rows[9];
	unsigned int cols[9];
	unsigned int blocks[9];
	char buffer[BOARD_BUFFER_SIZE];
};


/* helpful macros
 * ────────────────────────────────────────────────────────────────────────── */
#define IS_ASCII_DIGIT(DIGIT)	(((DIGIT) < ':') && ((DIGIT) > '/'))

/* 0..9 → '0'..'9' */
#define DIGIT_TO_ASCII(DIGIT)	(((unsigned int) (DIGIT)) | 48u)

/* '0'..'9' → 0..9 */
#define ASCII_TO_DIGIT(TOKEN)	(((unsigned int) (TOKEN)) & 15u)


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */



#endif /* ifndef SUDOKU_SUDOKU_H */

