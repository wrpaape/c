/**********************************************************
 *                              - b2a -                   *
 * Converts a "binary" file of ASCII "1"s and "0"s to its *
 * original ASCII representation                          *
 *                                                        *
 * b2a <binary file>                                      *
 **********************************************************/

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

#define BUFFER_SIZE (144 * 1024)    /* use 144K buffers */

char next_ascii_char(char **binary_buffer_ptr_ptr); /* performs b2a reversion */


int main(int argc, char *argv[])
{
  char *binary_buffer_ptr;            /* pointer to buffer for binary data */
  /* char binary_buffer[BUFFER_SIZE];            /1* pointer to buffer for binary data *1/ */
	char ascii_buffer[BUFFER_SIZE / 9]; /* buffer for ASCII data */
  int offset = 0;                     /* offset from start of 'ascii_buffer' */
	int file;		    	    		    	    /* file descriptor */
	int read_size;  				    	      /* number of bytes on last read */

  binary_buffer_ptr = malloc(BUFFER_SIZE);

  /* printf("BUFFER_SIZE: %i\n", BUFFER_SIZE); */
  /* printf("sizeof(*binary_buffer_ptr): %lu\n", BUFFER_SIZE); */

	if (argc != 2) {
    fprintf(stderr, "\e[31mERROR: wrong number of arguments\n");
    fprintf(stderr, "usage is: b2a <binary file>\e[0m\n");
    exit(8);
  }

  file = open(argv[1], O_RDWR | O_BINARY);

  if (file < 0) {
    fprintf(stderr, "\e[31mERROR: unable to open %s\n", argv[1]);
    exit(8);
  }

  while (1) {
    /* read_size = read(file, binary_buffer_ptr, BUFFER_SIZE); */
    read_size = read(file, binary_buffer_ptr, BUFFER_SIZE);

    if (read_size == 0) {
      printf("writing file:\n\n");

      ascii_buffer[offset] = '\0'; /* terminate 'ascii_buffer' */

      printf("%s", ascii_buffer);

      write(file, ascii_buffer, offset + 1);

      break; /* end of file */
    }

    if (read_size < 0) {
      fprintf(stderr, "\e[31mERROR: read error\n");
      exit(8);
    }

    while (*binary_buffer_ptr != '\0') {
      printf("*binary_buffer_ptr: %c\n", *binary_buffer_ptr);
      printf("offset: %i\n", offset);
      printf("sizeof(ascii_buffer): %lu\n", sizeof(ascii_buffer));

      ++binary_buffer_ptr; /* skip leading "bit" */

      ascii_buffer[offset] = next_ascii_char(&binary_buffer_ptr);

      printf("should be space *binary_buffer_ptr: %c\n", *binary_buffer_ptr);

      ++binary_buffer_ptr; /* skip space */

      /* printf("*binary_buffer_ptr: %hhi\n", *binary_buffer_ptr); */

      /* ++binary_buffer_ptr; /1* skip space *1/ */
      /* printf("*binary_buffer_ptr: %hhi\n", *binary_buffer_ptr); */
      /* ++binary_buffer_ptr; /1* skip space *1/ */
      printf("*binary_buffer_ptr: %hhi\n", *binary_buffer_ptr);
      close(file);
      return 0;
      ++offset; /* skip "binary" chunk and following space */
    }
  }

  close(file);

  return 0;
}


/*************************************************************
 *                 - next_ascii_char -                       *
 *                                                           *
 * Reads in next 7 "bits" of reerenced by 'binary_buffer_ptr' *
 * and returns the corresponding ASCII char                  *
 *************************************************************/
char next_ascii_char(char **binary_buffer_ptr_ptr)
{
  char ascii_char = 0; /* conversion result */

  for (int exp = 6; exp >= 0; --exp) {
      printf("    **binary_buffer_ptr_ptr: %c\n", **binary_buffer_ptr_ptr);
    if (**binary_buffer_ptr_ptr == '1') {
      ascii_char += (1 << exp);
    }
    
    ++(*binary_buffer_ptr_ptr);
  }
      printf("ascii_char: %c\n", ascii_char);
      printf("    **binary_buffer_ptr_ptr: %c\n", **binary_buffer_ptr_ptr);

  return ascii_char;
}
