#include <stdlib.h>	/* malloc, free, exit */
#include <stdio.h>	/* perror */
#include <unistd.h>	/* write */
#include <stdbool.h>	/* bool */



struct ColNode {
	unsigned int col;
	struct ColNode *restrict prev;
	struct ColNode *restrict next;
};

struct Board {
	char **rows;
	char *buffer;
	size_t size;
};

struct State {
	struct ColNode *rem_cols;
	unsigned int *queen_cols;
	unsigned int row;
	unsigned int n;
};


static inline void
exit_on_failure(const char *const restrict failure)
__attribute__((noreturn));

static inline void
exit_on_failure(const char *const restrict failure)
{
	perror(failure);
	exit(1);
	__builtin_unreachable();
}


static inline void
print_board(const struct Board *const restrict board,
	    const struct State *const restrict state)
{
	unsigned int row;

	char *const restrict *const restrict board_rows = board->rows;
	const unsigned int *const restrict queen_cols = state->queen_cols;
	const unsigned int n = state->n;


	row = 0;
	do {
		board_rows[row][queen_cols[row]] = 'X';
		++row;
	} while (row < n);


	if (write(STDOUT_FILENO,
		  board->buffer,
		  board->size) >= 0)
		return;

	exit_on_failure("print_board");
	__builtin_unreachable();
}


static inline void
init_board_state(struct Board *const restrict board,
		 struct State *const restrict state,
		 const unsigned int n)
{
	char *restrict *restrict board_row;
	char *restrict buffer;
	char *restrict buffer_until;
	struct ColNode *restrict prev_node;
	struct ColNode *restrict next_node;
	unsigned int col;

	board_row = board->rows;
	buffer	  = board->buffer;

	char *restrict *const restrict board_row_until = board_row + n;

	do {
		*board_row   = buffer;
		buffer_until = buffer + n;

		do {
			*buffer = 'O';
			++buffer;
		} while (buffer < buffer_until);

		*buffer = '\n';
		++buffer;

		++board_row;
	} while (board_row < board_row_until);

	next_node	= state->rem_cols;
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

	state->row = 0;
	state->n   = n;
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
queen_valid_fit(const unsigned int *const restrict queen_cols,
		const unsigned int new_row,
		const unsigned int new_col)
{
	unsigned int queen_row;
	unsigned int queen_col;

	unsigned int delta_row;
	unsigned int delta_col;

	queen_row = 0;

	while (1) {
		if (queen_row == new_row)
			return true;

		delta_row = new_row - queen_row;

		queen_col = queen_cols[queen_row];

		delta_col = (queen_col > new_col)
			  ? (queen_col - new_col)
			  : (new_col - queen_col);

		if (delta_row == delta_col)
			return false;

		++queen_row;
	}
}


bool
place_next_queen(struct State *const restrict state)
{
	struct ColNode *restrict node;

	const unsigned int queen_row = state->row;

	if (queen_row == state->n)
		return true;

	++(state->row);

	unsigned int *const restrict queen_col = &state->queen_cols[queen_row];

	node = state->rem_cols;

	if (queen_valid_fit(state->queen_cols,
			    queen_row,
			    node->col)) {

		*queen_col = node->col;

		state->rem_cols = node->next;

		if (place_next_queen(state))
			return true;

		state->rem_cols = node;
	}

	while (1) {
		node = node->next;

		if (node == NULL) {
			--(state->row);
			return false;
		}


		if (queen_valid_fit(state->queen_cols,
				    queen_row,
				    node->col)) {
			*queen_col = node->col;

			remove_col_node(node);

			if (place_next_queen(state))
				return true;

			replace_col_node(node);
		}
	}
}



static inline void
n_queens(const unsigned int n)
{
	struct Board board;
	struct State state;

	if (n == 0) {
		puts("no solution: n is zero");
		return;
	}

	board.size = sizeof(char) * (n * (n + 1));

	board.rows
	= malloc(  board.size
		 + (n * (  sizeof(char **)
			 + sizeof(struct ColNode)
			 + sizeof(unsigned int))));


	if (board.rows == NULL) {
		exit_on_failure("malloc");
		__builtin_unreachable();
	}

	board.buffer	 = (char *) (board.rows + n);
	state.rem_cols	 = (struct ColNode *) (board.buffer + board.size);
	state.queen_cols = (unsigned int *) (state.rem_cols + n);

	init_board_state(&board,
			 &state,
			 n);

	if (place_next_queen(&state))
		print_board(&board,
			    &state);
	else
		puts("no solution: did not find");

	free(board.rows);
	return;
}


int
main(void)
{
	n_queens(10);
	return 0;
}
