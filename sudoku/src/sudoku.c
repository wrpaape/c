#include "sudoku.h"

static struct Board board;

/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
exit_on_failure(const char *const restrict failure)
{
	file_write_muffle(STDERR_FILENO,
			  failure,
			  string_length(failure));
	exit(1);
	__builtin_unreachable();
}

static inline void
read_board(void)
{
	int file_descriptor;
	const char *restrict failure;

	if (LIKELY(file_open_report(&file_descriptor,
				    BOARD_PATH,
				    O_RDONLY,
				    &failure))) {

		if (LIKELY(file_read_report(file_descriptor,
					    &board.buffer[0],
					    BOARD_BUFFER_LENGTH,
					    &failure))) {

			if (LIKELY(file_close_report(file_descriptor,
						     &failure)))
				return;

		} else {
			file_close_muffle(file_descriptor);
		}
	}

	exit_on_failure(failure);
	__builtin_unreachable();
}


static inline void
init_board(void)
{
	read_board();


}

static inline void
print_board(void)
{
	const char *restrict failure;

	if (LIKELY(file_write_report(STDIN_FILENO,
				     &board.buffer[0],
				     BOARD_BUFFER_LENGTH,
				     &failure)))
		return;

	exit_on_failure(failure);
	__builtin_unreachable();
}

static inline void
print_failed_to_solve(void)
{
	const char *restrict failure;

	if (LIKELY(file_write_report(STDIN_FILENO,
				     "failed to solve\n",
				     sizeof("failed to solve\n"),
				     &failure)))
		return;

	exit_on_failure(failure);
	__builtin_unreachable();
}


bool
solve_board(void)
{
	unsigned int move;

	if (board.rem_cells == board.cells_until)
		return true;

	struct Cell *const restrict cell = board.rem_cells;
	++board.rem_cells;

	const unsigned int rem_moves = !(  *(cell->row)
					 | *(cell->col)
					 | *(cell->block));

	move = 1 << 1;

	while (1) {
		if (rem_moves & move) {
			*(cell->row)   |= move;
			*(cell->col)   |= move;
			*(cell->block) |= move;

			if (solve_board()) {
				*(cell->token) = MOVE_TO_ASCII(move);
				return true;
			}

			*(cell->row)   ^= move;
			*(cell->col)   ^= move;
			*(cell->block) ^= move;
		}

		if (move == (1 << 9))
			return false;

		move <<= 1;
	}
}


/* program entry
 * ────────────────────────────────────────────────────────────────────────── */
int
main(void)
{
	init_board();

	print_board();

	if (solve_board())
		print_board();
	else
		print_failed_to_solve();

	return 0;
}
