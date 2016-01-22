#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char INPUT_FORMAT[]     = "\e[37m<string>\e[0m/\e[35m<find_str>\e[0m/\e[36m<replace_str>\e[0m";
const char INPUT_ERROR_MSG[]  = "\e[31mERROR pls delimit ur input with slashes accordings to:\e[0m";

char *gets_trimmed(char *input_buffer, int buffer_size); /* retrieve and trim user input */
char *my_strchar(char *string_ptr, char find_char);      /* copycat 'strchar' from string.h */

int main(void)
{
  char input[80];    /* input to be processed */
  char *string_ptr;  /* pointer to string */
  char *find_ptr;    /* pointer to target substring */
  char *replace_ptr; /* pointer to replacement substring */

  string_ptr = gets_trimmed(input, sizeof(input)); /* point 'string_ptr' at beginning of input */

  printf("string_ptr:  %s\n",  string_ptr);
  /* printf("find_ptr:    %s\n",    find_ptr); */
  /* printf("replace_ptr: %s\n", replace_ptr); */
  
  

  /* string_ptr = input;                /1* point string_ptr at beginning of input *1/ */


/*   find_ptr = my_strchar(input, '/'); /1* locate first slash in input *1/ */

/*   if (find_ptr == NULL) { */
/*     fprintf(stderr, "\n  %s\n\n%s", INPUT_ERROR_MSG, INPUT_FORMAT); /1* exit on input error is no slash found *1/ */

/*     exit(8); */
/*   } */

/*   *find_ptr = '\0'; /1* dereference pointer to slash and replace with null char *1/ */

/*   ++find_ptr;       /1* inc pointer so that it now points to beginning of find_str *1/ */


/*   replace_ptr = my_strchar(find_ptr, '/'); /1* locate next slash in input *1/ */

/*   if (replace_ptr == NULL) { */
/*     fprintf(stderr, "\n%s\n\n  %s\n", INPUT_ERROR_MSG, INPUT_FORMAT); /1* exit on input error is no slash found *1/ */

/*     exit(8); */
/*   } */

/*   *replace_ptr = '\0'; /1* dereference pointer to slash and replace with null char *1/ */

/*   ++replace_ptr;       /1* inc pointer so that it now points to beginning of replace_str *1/ */


/*   int length_string   = strlen(*string_ptr); /1* length of total string *1/ */
/*   int length_find_str = strlen(*find_ptr);   /1* length of target substring *1/ */

/*   int string_index = 0; */
/*   int match_index  = 0; */


/*   while ((strlen(*string_ptr) - string_index) >= strlen(*find_ptr)) { */


/*   } */


  return 0;
} 

/************************************
 *         - gets_trimmed -         *
 *                                  *
 * retrieves user input from stdin  *
 * and removes the trailing newline *
 ************************************/
char *gets_trimmed(char *input_buffer, int buffer_size) {

  printf("%s\n  \e[5m>\e[25m ", INPUT_FORMAT); /* prompt user for input */

  fgets(input_buffer, buffer_size, stdin);     /* set user input to 'input_buffer' */

  input_buffer[strlen(input_buffer) - 1] = '\0';      /* trim trailing \n */

  return input_buffer;
}


/************************************************
 *                - my_strchar -                *
 *                                              *
 * loops through string referenced by           *
 * 'string_ptr' until char 'find_char'          *
 * is found                                     *
 *                                              *
 * string_ptr is incremented accordingly and    *
 * set to 'NULL' if 'find_char' is not found    *
 *                                              *
 * clone of standard library function 'strchar' *
 ************************************************/
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
