#include <stdlib.h>	/* malloc, free, exit */
#include <stdio.h>	/* perror */
#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */



struct ColNode {
	unsigned int col;
	struct ColNode *restrict prev;
	struct ColNode *restrict next;
};

struct Queen {
	unsigned int row;
	unsigned int col;
};

struct QueenBuffer {
	struct Queen *restrict from;
	struct Queen *restrict until;
};


struct Board {
	char *buffer;
	size_t size;
};

struct RowState {
	char **row;
	char **until;
	struct ColNode *cols;
};


static inline void
exit_on_failure(const char *const restrict failure)
__attribute__((no_return));

static inline void
exit_on_failure(const char *const restrict failure)
{
	perror(failure);
	exit(1);
	__builtin_unreachable();
}


static inline void
print_board(const struct Board *const restrict board)
{
	if (write(STDOUT_FILENO,
		  board->buffer,
		  board->size) >= 0)
		return;

	exit_on_failure("print_board");
	__builtin_unreachable();
}


static inline void
init_board_row_state(struct Board *const restrict board,
		     struct RowState *const restrict row_state,
		     const unsigned int n)
{
	char *restrict *restrict row;
	char *restrict buffer;
	char *restrict buffer_until;
	struct ColNode *restrict prev_node;
	struct ColNode *restrict next_node;
	unsigned int col;

	buffer = board->buffer;
	row    = row_state->row;

	do {
		*row	     = buffer;
		buffer_until = buffer + n;

		do {
			*buffer = 'O';
			++buffer;
		} while (buffer < buffer_until);

		*buffer = '\n';
		++buffer;

		++row;
	} while (row < row_state->until);

	next_node	= row_state->cols;
	next_node->col	= 0;
	next_node->prev = NULL;

	for (col = 1; col < n; ++col) {
		prev_node = next_node;
		++next_node;

		prev_node->next = next_node;
		next_node->col	= col;
		next_node->prev = prev_node;
	}

	next_node->next = NULL;
}

static inline void
remove_col_node(struct ColNode *const restrict node)
{
	struct ColNode *const restrict prev = node->prev;
	struct ColNode *const restrict next = node->next;

	if (prev != NULL)
		prev->next = next;

	if (next != NULL)
		next->prev = prev;
}

static inline void
replace_col_node(struct ColNode *const restrict node)
{
	struct ColNode *const restrict prev = node->prev;
	struct ColNode *const restrict next = node->next;

	if (prev != NULL)
		prev->next = node;

	if (next != NULL)
		next->prev = node;
}

static inline bool
queens_collide_diagonally(const struct Queen *const restrict queen1,
			  const struct Queen *const restrict queen2)
{
	return (queen2->row - queen1->row) == (queen2->col - queen1->col);
}

static inline bool
queen_valid_fit(const struct QueenBuffer *const restrict buffer,
		const struct Queen *const restrict new_queen)
{
	const struct Queen *restrict queen;

	queen = buffer->from;

	while (1) {
		if (queen == buffer->until)
			return true;

		if (queens_collide_diagonally(queen,
					      new_queen))
			return false;

		++queen;
	}
}



bool
place_next_queen(struct Board *const restrict board,
		 struct RowState *const restrict row_state)
{
	struct ColNode *restrict node;
	char *restrict cell;

	if (row_state->row = row_state->until)
		return true;

	node = row_state->cols;

	if (node == NULL)
		return false;

	cell = &board->buffer[node->col];

	*cell = 'X';

	row_state->cols = node->next;
	++(row_state->row);
	if (place_next_queen(board,
			     row_state))
		return true;




}


static inline void
n_queens(const unsigned int n)
{
	struct Board board;
	struct RowState row_state;

	if (n == 0) {
		puts("no solution: n is zero");
		return;
	}


	board.size = sizeof(char) * (n * (n + 1));

	board.buffer
	= malloc(board.size + (n * (sizeof(char **) + sizeof(struct ColNode))));


	if (board.buffer == NULL) {
		exit_on_failure("malloc");
		__builtin_unreachable();
	}

	row_state.row	= (char **) (board.buffer + board.size);
	row_state.until	= row_state.row + n;
	row_state.cols	= (struct ColNode *) row_state.until;


	init_board_row_state(&board,
			     &row_state,
			     n);

	if (place_next_queen(&board,
			     &row_state))
		print_board(&board);
	else
		puts("no solution: did not find");

	free(board.buffer);
	return;
}


int
main(void)
{
	n_queens(8);
	return 0;
}







