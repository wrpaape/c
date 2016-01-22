#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char input_format[] = "\e[37m<string>\e[0m/\e[35m<find_char>\e[0m/\e[36m<replace_char>\e[0m";
const char input_error[]  = "\e[31mERROR pls delimit ur input with slashes accordings to:\e[0m";

int main(void)
{
  char input[80];    /* input to be processed */
  char *string_ptr;  /* pointer to string */
  char *find_ptr;    /* pointer to target substring */
  char *replace_ptr; /* pointer to replacement substring */

  char *my_strchar(char *string_ptr, char find_char); /* copycat 'strchar' from string.h */

  printf("%s\n  \e[5m>\e[25m ", input_format);
  fgets(input, sizeof(input), stdin);

  input[strlen(input) - 1] = '\0';   /* trim trailing \n */

  string_ptr = input;                /* point string_ptr at beginning of input */


  find_ptr = my_strchar(input, '/'); /* locate first slash in input */

  if (find_ptr == NULL) {
    fprintf(stderr, "\n  %s\n\n%s", input_error, input_format); /* exit on input error is no slash found */

    exit(8);
  }

  *find_ptr = '\0'; /* dereference pointer to slash and replace with null char */

  ++find_ptr;       /* inc pointer so that it now points to beginning of find_str */


  replace_ptr = my_strchar(find_ptr, '/'); /* locate next slash in input */

  if (replace_ptr == NULL) {
    fprintf(stderr, "\n%s\n\n  %s\n", input_error, input_format); /* exit on input error is no slash found */

    exit(8);
  }

  *replace_ptr = '\0'; /* dereference pointer to slash and replace with null char */

  ++replace_ptr;       /* inc pointer so that it now points to beginning of replace_str */

  printf("string_ptr:  %s\n",  string_ptr);
  printf("find_ptr:    %s\n",    find_ptr);
  printf("replace_ptr: %s\n", replace_ptr);
  
  
  return 0;
} 


char *my_strchar(char *string_ptr, char find_char)
{
  while (*string_ptr != find_char) {
    if (*string_ptr == '\0') {
      return NULL;   /* 'find_char' not found */
    }

    ++string_ptr;
  }

  return string_ptr; /* string_ptr now points at find_char */
}
