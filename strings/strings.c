#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
  char string[80];   /* string to be processed */
  char find_char;    /* char to find and replace with 'replace_char' */
  char replace_char; /* replacement char */
  char *first_ptr;
  char *last_ptr;

  printf("<string>/<find_char>/<replace_char>\n  \e[5m>\e[25m ");
  fgets(string, sizeof(string), stdin);

  string[strlen(line) - 1] = '\0'; /* trim trailing \n */
  
  
  
  return 0;
} 
