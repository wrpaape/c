#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char INPUT_FORMAT[]     = "\e[37m<string>\e[0m/\e[35m<find_str>\e[0m/\e[36m<replace_str>\e[0m";
const char INPUT_ERROR_MSG[]  = "\e[31mERROR pls delimit ur input with slashes accordings to:\e[0m";

char *gets_trimmed(char *input_buffer, int buffer_size); /* retrieve and trim user input */
char *split_slash(char *string_ptr);                     /* returns pointer to char after next slash */

int main(void)
{
  char input[80];    /* input to be processed */
  char *string_ptr;  /* pointer to string */
  char *find_ptr;    /* pointer to target substring */
  char *replace_ptr; /* pointer to replacement substring */

  string_ptr =  gets_trimmed(input, sizeof(input)); /* point 'string_ptr' at beginning of input */

  find_ptr =    split_slash(string_ptr);            /* trim string and set pointer to start of find_str */

  replace_ptr = split_slash(find_ptr);              /* trim find_str and set pointer to start of replace_str */


  /* int length_string   = strlen(*string_ptr); /1* length of total string *1/ */
  /* int length_find_str = strlen(*find_ptr);   /1* length of target substring *1/ */

  /* int string_index = 0; */
  /* int match_index  = 0; */


  /* while ((strlen(*string_ptr) - string_index) >= strlen(*find_ptr)) { */


  /* } */

  printf("string_ptr:  %s\n",  string_ptr);
  printf("find_ptr:    %s\n",    find_ptr);
  printf("replace_ptr: %s\n", replace_ptr);
  


  return 0;
} 

/************************************
 *         - gets_trimmed -         *
 *                                  *
 * retrieves user input from stdin  *
 * and removes the trailing newline *
 ************************************/
char *gets_trimmed(char *input_buffer, int buffer_size)
{
  printf("%s\n  \e[5m>\e[25m ", INPUT_FORMAT); /* prompt user for input */

  fgets(input_buffer, buffer_size, stdin);     /* set user input to 'input_buffer' */

  input_buffer[strlen(input_buffer) - 1] = '\0';      /* trim trailing \n */

  return input_buffer;
}


/*************************************************************************
 *                            - split_slash -                            *
 *                                                                       *
 * loops through string referenced by 'string_ptr' until slash is found  *
 *                                                                       *
 * slash is set to '\0' and 'string_ptr' is incremented to point at char *
 * immediately following slash                                           *
 *                                                                       *
 * if no slash is found, the program exits with status '8'               *
 *                                                                       *
 * implements a clone of standard library function 'strchar'             *
 *************************************************************************/
char *split_slash(char *string_ptr)
{
  while (*string_ptr != '/') {
    if (*string_ptr == '\0') {
      fprintf(stderr, "\n  %s\n\n%s", INPUT_ERROR_MSG, INPUT_FORMAT); /* exit on input error is no slash found */

      exit(8);
    }

    ++string_ptr;
  }


  *string_ptr = '\0'; /* dereference pointer to slash and replace with null char */

  ++string_ptr;       /* inc pointer so that it now points to char immediately after slash*/

  return string_ptr; /* string_ptr now points at find_char */
}
