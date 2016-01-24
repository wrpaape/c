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

#define BUFFER_SIZE (16 * 1024)    /* use 16K buffers */

/* char next_ascii_char(char **binary_buffer_ptr); /1* performs b2a reversion *1/ */


int main(int argc, char *argv[])
{
  char binary_buffer[BUFFER_SIZE]; /* buffer for binary data */
	char ascii_buffer[BUFFER_SIZE];  /* buffer for ASCII data */
  char ascii_char;                 /* conversion result */
  int binary_offset;               /* offset from start of 'binary_buffer' */
  int ascii_offset;                /* offset from start of 'ascii_buffer' */
  int exp;                         /* exponentiation according to bit position */
	int file;		    	    		    	 /* file descriptor */
	int read_size;  				    	   /* number of bytes on last read */


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

  ascii_offset = 0;

  while (1) {
    read_size = read(file, binary_buffer, sizeof(binary_buffer));

    if (read_size == 0) {
      printf("writing file:\n\n");

      ascii_buffer[ascii_offset] = '\0'; /* terminate 'ascii_buffer' */

      printf("%s", ascii_buffer);

      write(file, ascii_buffer, ascii_offset + 1);

      break; /* end of file */
    }

    if (read_size < 0) {
      fprintf(stderr, "\e[31mERROR: read error\n");
      exit(8);
    }

    binary_offset = 0;

    while (binary_buffer[binary_offset] != '\0') {
      ++binary_offset; /* skip leading "bit" */

      ascii_char = 0;

      for (exp = 6; exp >= 0; --exp) {
        if (binary_buffer[binary_offset] == '1') {
          ascii_char += (1 << exp);
        }
        
        ++binary_offset;
      }

      ++binary_offset; /* skip space */

      /* printf("*binary_buffer_ptr: %hhi\n", *binary_buffer_ptr); */

      /* ++binary_buffer_ptr; /1* skip space *1/ */
      /* printf("*binary_buffer_ptr: %hhi\n", *binary_buffer_ptr); */
      /* ++binary_buffer_ptr; /1* skip space *1/ */
      /* printf("*binary_buffer_ptr: %hhi\n", *binary_buffer_ptr); */
      /* close(file); */
      /* return 0; */

      ascii_buffer[ascii_offset] = ascii_char;
      ++ascii_offset;
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
/* char next_ascii_char(char *binary_buffer_ptr) */
/* { */
/*   char ascii_char = 0; /1* conversion result *1/ */

/*   for (int exp = 6; exp >= 0; --exp) { */
/*     if (*binary_buffer_ptr == '1') { */
/*       ascii_char += (1 << exp); */
/*     } */
    
/*     ++binary_buffer_ptr; */
/*   } */

/*   return ascii_char; */
/* } */
