#include <unistd.h>	/* read/write */
#include <stdlib.h>	/* exit */
#include <stdio.h>	/* printf */


#define BUFFER_SIZE 1024

#define ASCII_TO_DIGIT(ASCII) (((unsigned int) (ASCII)) & 15u)








void
read_expression(char *const restrict buffer,
		const size_t size_max)
{
	const char *restrict failure;

	const ssize_t size_read = read(STDIN_FILENO,
				       buffer,
				       size_max);

	if (size_read < 0) {
		failure = "read failure\n";
	} else if (size_read == size_max) {
		failure = "read overflow\n";
	} else {
		buffer[size_read - 1] = '\0';	/* remove newline */
		return;
	}

	fputs(failure,
	      stderr);
	exit(1);
	__builtin_unreachable();
}




int
main(void)
{
	static char buffer[BUFFER_SIZE];

	read_expression(&buffer[0],
			sizeof(buffer));

	fputs(&buffer[0],
	      stdout);

	return 0;
}
