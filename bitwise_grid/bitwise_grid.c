#include <stdio.h>
#include <string.h>

#define X_RES 40  /* resolution in X direction */
#define Y_RES 20  /* resolution in Y direction */
/* 
 *  This program sets and prints a basic graphics bit array.
 */

char grid[X_RES / 8][Y_RES];             /* 
                                          * need 1/8th of actual X resolution
                                          * for storage because each char byte
                                          * will be implemented as a miniture array
                                          */

int main()
{
  void set_grid(void);                   /* function for initializing grid array */
  void print_grid(void);                 /* function for printing grid array */

  set_grid();
  print_grid();

  return 0;
}


/***********************************************************
 *                       set_grid                          *
 * sets grid array with alternating rows of 1s and 0s bits *
 ***********************************************************/
void set_grid()
{
  int x, y;                              /* x and y screen coordinates */
  void flip_bit(int x, int y);           /* 
                                          * function for setting bit in grid
                                          * array at specified coordinates
                                          */
  for (x = 0; x < X_RES; x += 2) {
    for (y = 0; y < Y_RES; ++y) {
      flip_bit(x, y);
    }
  }
}


/*********************************************
 *               flip_bit                    *
 * maps a 1 bit at screen coordinates (x, y) *
 *********************************************/
void flip_bit(int x, int y)
{
  int byte_x = x / 8;                    /* byte coordinate in X direction */
  int bit_index = x % 8;                 /*
                                          * index of bit to be flipped at byte_x
                                          * of grid
                                          */
  int byte_with_bit = 0x80 >> bit_index; /*
                                          * empty byte except for shifted bit to be
                                          * ord with char byte at byte coordinates
                                          * of grid
                                          */

  grid[byte_x][y] |= byte_with_bit;
}


/******************************************
 *             print_grid                 *
 * prints the grid bit array as 1s and 0s *
 ******************************************/
void print_grid()
{
  int x;                                 /* screen coordinate in X direction */
  int y;                                 /* screen coordinate in Y direction */
  unsigned char bit_x;                   /* bit coordinate in X direction */

	for (y = 0; y < Y_RES; ++y) {
		for (x = 0; x < X_RES / 8; ++x) {
      for (bit_x = 0x80; bit_x > 0; bit_x = (bit_x >> 1)) {
        if ((grid[x][y] & bit_x) != 0) {
          printf("\e[32m1\e[0m");
        } else {
          printf("\e[31m0\e[0m");
        }
      }
    }
    printf("\n");
  }
}

