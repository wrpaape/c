/* 
 * a program for getting familiar with and manipulating c arrays
 */

#include <stdio.h>

int main() {
  int array_length = 10;
  char array[array_length];

  for (int i = 0; i < array_length; ++i) {
    array[i] = 100 + i;
  }

  int i = array_length;

  while (i >= 0) {
    printf("array index: %d, value: %c\n", i, array[i]);
    --i;
  }

  return(0);
}
