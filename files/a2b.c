/*********************************************************************
 *                              - a2b -                              *
 * Converts an ASCII file to a "binary" file of ASCII "1"s and "0"s. *
 *                                                                   *
 * a2b <read file> <write file>                                      *
 *********************************************************************/

#include <stdio.h>

#ifndef __MSDOS__                   /* if we are not MS-DOS */
#define __UNIX__                    /* then we are UNIX */
#endif

#include <stdlib.h>

#ifdef __UNIX__
#include <sys/types.h>              /* file defines for UNIX filesystem */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef __MSDOS__
#include <fcntl.h>                  /* file defines for DOS filesystem */
#include <sys\stat.h>
#include <io.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0                  /* define the flag if not defined yet */
#endif

#define BUFFER_SIZE (16 * 1024)     /* use 16K buffers */

void convert(char *binary_char_ptr, char ascii_char); /* performs a2b char conversion */


int main(int argc, char *argv[])
{
	char ascii_buffer[BUFFER_SIZE];      /* buffer for ASCII data */
	char binary_buffer[9 * BUFFER_SIZE]; /* buffer for "binary" data */
	int in_file;							           /* input file descriptor */
	int out_file;							           /* output file descriptor */
	int read_size;					             /* number of bytes on last read */

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
    read_size = read(in_file, ascii_buffer, sizeof(ascii_buffer));

    if (read_size == 0) {
      break; /* end of file */
    }

    if (read_size < 0) {
      fprintf(stderr, "\e[31mERROR: read error\n");
      exit(8);
    }

    for (int i = 0; i < sizeof(ascii_buffer); ++i) {
      convert(&binary_buffer[9 * i], ascii_buffer[i]);
    }

    write(out_file, binary_buffer, (unsigned int) 9 * read_size);
  }

  close(in_file);
  close(out_file);

  return 0;
}


/*******************************************************
 *                     - convert -                     *
 *                                                     *
 * Converts ASCII char referenced by 'ascii_char_ptr'. *
 * to "binary" "1"s and "0"s.                          *
 *******************************************************/
void convert(char *binary_char_ptr, char ascii_char)
{
  unsigned int bit; /* test bit used to scan ascii_char */

  for (bit = 0x80; bit > 0; bit = (bit >> 1)) {
    *binary_char_ptr = (ascii_char & bit) != 0 ? '1' : '0';

    ++binary_char_ptr;
  }

  *binary_char_ptr = ' ';
}
