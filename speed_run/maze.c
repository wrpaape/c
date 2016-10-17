#include <assert.h>	/* assert */
#include <stdio.h>	/* puts */

struct Cell {
	int row;
	int col;
};

struct Moveset {
	struct Cell next[4];
	const struct Cell *restrict until;
};

static int maze1row1[5] = {0, 0, 1, 0, 0};
static int maze1row2[5] = {0, 0, 0, 0, 1};
static int maze1row3[5] = {1, 1, 1, 0, 0};
static int maze1row4[5] = {1, 0, 1, 0, 0};
static int maze1row5[5] = {0, 0, 0, 0, 0};

static int *maze1[5] = {
	&maze1row1[0],
	&maze1row2[0],
	&maze1row3[0],
	&maze1row4[0],
	&maze1row5[0]
};


const struct Cell exit1 = {
	.row = 3u,
	.col = 1u
};

const int min_steps1 = 10;

static int maze2row1[5] = {0, 0, 1, 0, 0};
static int maze2row2[5] = {0, 0, 1, 0, 1};
static int maze2row3[5] = {1, 1, 1, 0, 0};
static int maze2row4[5] = {1, 0, 1, 0, 0};
static int maze2row5[5] = {0, 0, 0, 0, 0};

static int *maze2[5] = {
	&maze2row1[0],
	&maze2row2[0],
	&maze2row3[0],
	&maze2row4[0],
	&maze2row5[0]
};

const struct Cell exit2 = {
	.row = 3u,
	.col = 1u
};

const int min_steps2 = -1;

static int maze3row1[5] = {0, 1, 1, 0, 0};
static int maze3row2[5] = {0, 0, 0, 0, 1};
static int maze3row3[5] = {1, 1, 0, 1, 0};
static int maze3row4[5] = {0, 0, 0, 0, 0};
static int maze3row5[5] = {0, 0, 0, 1, 0};

static int *maze3[5] = {
	&maze3row1[0],
	&maze3row2[0],
	&maze3row3[0],
	&maze3row4[0],
	&maze3row5[0]
};

const struct Cell exit3 = {
	.row = 4u,
	.col = 4u
};

const int min_steps3 = 8;


int
is_exit(const struct Cell *const restrict point)
{
	return (point->row == 0)
	    && (point->col == 0);
}

void
next_moves(struct Moveset *const restrict moves,
	   const struct Cell *const restrict current,
	   const struct Cell *const restrict bounds,
	   int **maze)
{
	int next_row;
	int next_col;

	struct Cell *restrict cell;

	cell = &moves->next[0];

	next_row = current->row - 1;
	next_col = current->col;

	/* printf("next move: { row: %d, col: %d, maze: %d }\n", next_row, next_col, maze[next_row][next_col]); */
	/* fflush(stdout); */

	if ((next_row >= 0) && (maze[next_row][next_col] == 0)) {
		cell->row = next_row;
		cell->col = next_col;
		++cell;
	}

	next_row += 2;
	/* printf("next move: { row: %d, col: %d, maze: %d }\n", next_row, next_col, maze[next_row][next_col]); */
	/* fflush(stdout); */

	if ((next_row < bounds->row) && (maze[next_row][next_col] == 0)) {
		cell->row = next_row;
		cell->col = next_col;
		++cell;
	}

	next_row = current->row;
	--next_col;
	/* printf("next move: { row: %d, col: %d, maze: %d }\n", next_row, next_col, maze[next_row][next_col]); */
	/* fflush(stdout); */

	if ((next_col >= 0) && (maze[next_row][next_col] == 0)) {
		cell->row = next_row;
		cell->col = next_col;
		++cell;
	}

	next_col += 2;
	/* printf("next move: { row: %d, col: %d, maze: %d }\n", next_row, next_col, maze[next_row][next_col]); */
	/* fflush(stdout); */

	if ((next_col < bounds->col) && (maze[next_row][next_col] == 0)) {
		cell->row = next_row;
		cell->col = next_col;
		++cell;
	}

	moves->until = cell;
}

int
do_min_steps(int **maze,
	     const struct Cell *const restrict bounds,
	     const struct Cell *const restrict current)
{
	int min_steps;
	int steps;
	struct Moveset moves;
	struct Cell *restrict cell;
	int *next;

	if (is_exit(current))
		return 0;

	/* printf("finding next_moves from: { %d, %d }\n", */
	/*        current->row, */
	/*        current->col); */
	/* fflush(stdout); */

	next_moves(&moves,
		   current,
		   bounds,
		   maze);

	cell = &moves.next[0];


	while (1) {
		if (cell == moves.until)
			return -1;

		next = &maze[cell->row][cell->col];

		*next = 1;
		min_steps = do_min_steps(maze,
					 bounds,
					 cell);
		*next = 0;

		++cell;

		if (min_steps >= 0)
			break;
	}


	while (cell < moves.until) {
		next = &maze[cell->row][cell->col];

		*next = 1;
		steps = do_min_steps(maze,
				     bounds,
				     cell);
		*next = 0;

		if ((steps >= 0) && (steps < min_steps))
			min_steps = steps;

		++cell;
	}

	return min_steps + 1;
}


int
min_steps(int **maze,
	  const int rows,
	  const int cols,
	  const int row_exit,
	  const int col_exit)
{
	const struct Cell bounds = {
		.row = rows,
		.col = cols
	};

	const struct Cell current = {
		.row = row_exit,
		.col = col_exit
	};

	return do_min_steps(maze,
			    &bounds,
			    &current);
}

int
main(void)
{
	int steps;

	steps = min_steps(&maze1[0],
			  5,
			  5,
			  exit1.row,
			  exit1.col);

	printf("steps for maze1: %d/%d\n", steps, min_steps1);
	assert(steps == min_steps1);
	puts("passed");

	steps = min_steps(&maze2[0],
			  5,
			  5,
			  exit2.row,
			  exit2.col);

	printf("steps for maze2: %d/%d\n", steps, min_steps2);
	assert(steps == min_steps2);
	puts("passed");

	steps = min_steps(&maze3[0],
			  5,
			  5,
			  exit3.row,
			  exit3.col);

	printf("steps for maze3: %d/%d\n", steps, min_steps2);
	assert(steps == min_steps3);
	puts("passed");

	return 0;
}
