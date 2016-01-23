/*****************************************************************
 *                            - a2b -                            *
 * Converts an ASCII file to a 'binary' file of ASCII 1s and 0s. *
 *                                                               *
 * a2b <read file> <write file>                                  *
 *****************************************************************/

#include <stdio.h>

#ifndef __MSDOS__               /* if we are not MS-DOS */
#define __UNIX__                /* then we are UNIX */
#endif

#include <stdlib.h>

#ifdef __UNIX__
#include <sys/types.h>          /* file defines for UNIX filesystem */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef __MSDOS__
#include <fcntl.h>              /* file defines for DOS filesystem */
#include <sys\stat.h>
#include <io.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0              /* define the flag if not defined yet */
#endif

#define BUFFER_SIZE (16 * 1024) /* use 16K buffers */


int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE]; /* buffer for data */
	int in_file;							/* input file descriptor */
	int out_file;							/* output file descriptor */
	int read_size;					  /* number of bytes on last read */

	if (argc != 3) {
    fprintf(stderr, "\e[31mERROR: wrong number of arguments\n");
    fprintf(stderr, "usage is: a2b <read file> <write file>\e[0m\n");
    exit(8);
  }

  in_file = open(argv[1], O_RDONLY | O_BINARY);

  if (in_file < 0) {
    fprintf(stderr, "\e[31mERROR: unable to open %s\n", argv[1]);
    exit(8);
  }

  out_file = open(argv[2], O_WRONLY| O_TRUNC | O_CREAT | O_BINARY, 0666);

  if (out_file < 0) {
    fprintf(stderr, "\e[31mERROR: unable to open %s\n", argv[2]);
    exit(8);
  }

  while (1) {
    read_size = read(in_file, buffer, sizeof(buffer));

    if (read_size == 0) {
      break; /* end of file */
    }

    if (read_size < 0) {
      fprintf(stderr, "\e[31mERROR: read error\n");
      exit(8);
    }

    write(out_file, buffer, (unsigned int) read_size);
  }

  close(in_file);
  close(out_file);

  return 0;
}
