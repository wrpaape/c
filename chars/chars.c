#include <stdio.h>
#include <string.h>

char string[4];

int main(int argc, char *argv[])
{
  strcpy(string, "aBc");

  printf("%d arguments\n", argc);

  for (int i = 0; i < argc; ++i) {
    printf("* %s *\n", argv[i]);
  }

  printf("\n\nwhole string: %s\n", string);

  for (int i = 0; i < 4; ++i) {
    printf("index %d, char: %c\n", i, string[i]);
  }

  printf("strlen of %s: %lu\n", string, strlen(string));
  
  return(0);
}
