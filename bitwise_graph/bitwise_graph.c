#include <stdio.h>

#define X_RES 80  /* resolution in X direction */
#define Y_RES 40  /* resolution in Y direction */
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

void set_grid()
{
  int x, y;                              /* x and y screen coordinates */
  void flip_bit(int x, int y);           /* 
                                          * function for setting bit in grid
                                          * array at specified coordinates
                                          */
  for (x = 0; x < X_RES; ++x) {
    for (y = 0; y < Y_RES; y += 2) {
      flip_bit(x, y);
    }
  }
}


void flip_bit(int x, int y)
{
  int byte_x = x / 8;                    /* byte coordinate in X direction */
  int byte_y = y;                        /* byte coordinate in Y direction */
  int bit_index = x % 8;                 /*
                                          * index of bit to be flipped at byte_x
                                          * of grid
                                          */
  int byte_with_bit = 0x80 >> bit_index; /*
                                          * empty byte except for shifted bit to be
                                          * ord with char byte at byte coordinates
                                          * of grid
                                          */

  grid[byte_x][byte_y] |= byte_with_bit;
}

void print_grid()
{
  for (x = 0; x < X_RES; ++x) {
    for (y = 0; y < Y_RES; y += 2) {
      flip_bit(x, y);
    }
  }
}
