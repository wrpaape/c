#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BUFFER_SIZE 80

struct Results {
  char *string_ptr;
  int replace_count;
};

const char INPUT_FORMAT[]    = "\e[37m<string>\e[0m/\e[35m<find_str>\e[0m/\e[36m<replace_str>\e[0m";
const char INPUT_ERROR_MSG[] = "\e[31mERROR pls delimit ur input with slashes accordings to:\e[0m";

char *gets_trimmed(char *input_buffer, int buffer_size);                      /* retrieve and trim user input */
char *split_slash(char *string_ptr);                                          /* returns pointer to char after next slash */
struct Results find_and_replace(const char *string_ptr, const char *find_ptr, const char *replace_ptr);  /* global find and replace */
void report_results(char *final_ptr, int num_replaced, clock_t time_elapsed); /* prints program results */


int main(void)
{
  struct Results results;
  clock_t start;                  /* time of program start */
  clock_t finish;                 /* time of program finish */
  char input[MAX_BUFFER_SIZE];    /* input to be processed */
  char *string_ptr;               /* pointer to string */
  char *find_ptr;                 /* pointer to target substring */
  char *replace_ptr;              /* pointer to replacement substring */
  /* char *results_ptr; /1* pointer to array of results from 'find_and_replace' *1/ */

  string_ptr  = gets_trimmed(input, sizeof(input));                  /* point 'string_ptr' at beginning of input */

  start       = clock();                                             /* start time */

  find_ptr    = split_slash(string_ptr);                             /* trim string and set pointer to start of find_str */

  replace_ptr = split_slash(find_ptr);                               /* trim find_str and set pointer to start of replace_str */

  results = find_and_replace(string_ptr, find_ptr, replace_ptr); /* perform global find and replace */

  finish      = clock();                                             /* finish time */

  /* report_results(*results_ptr, *(results_ptr + 1), finish - start);    /1* print program results *1/ */

  return 0;
} 


/********************************************************************
 *                         - gets_trimmed -                         *
 *                                                                  *
 * retrieves user input from stdin and removes the trailing newline *
 ********************************************************************/
char *gets_trimmed(char *input_buffer, int buffer_size)
{
  printf("%s\n  \e[5m>\e[25m ", INPUT_FORMAT);   /* prompt user for input */

  fgets(input_buffer, buffer_size, stdin);       /* set user input to 'input_buffer' */

  input_buffer[strlen(input_buffer) - 1] = '\0'; /* trim trailing \n */

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

  return string_ptr;  /* string_ptr now points at find_char */
}


/****************************************************************************
 *                           - find_and_replace -                           *
 *                                                                          *
 * loops through string indicated by 'string_ptr', replacing instances of   *
 * substring indicated by 'find_ptr' with string indicated by 'replace_ptr' *
 ****************************************************************************/
struct Results find_and_replace(const char *string_ptr, const char *find_ptr, const char *replace_ptr)
{
  char *final_string_ptr = (char *) malloc(MAX_BUFFER_SIZE);
  struct Results results = {final_string_ptr, 0};

  int length_find_str    = strlen(find_ptr);    /* length of target substring */
  int length_replace_str = strlen(replace_ptr); /* length of replacement string */

  int match_count  = 0; /* counter tracking the consecutive matches of target substring */

  while (*string_ptr != '\0') {
    if (*string_ptr == *find_ptr) {
      ++match_count;
      ++find_ptr;

      /* printf("%c matches %c\n", *string_ptr, *find_ptr); */
      /* printf("match_count: %u\n", match_count); */
      /* printf("length_find_str: %u\n", length_find_str); */

      if (match_count == length_find_str) {

        while(*replace_ptr != '\0') {
          *final_string_ptr = *replace_ptr;

          ++final_string_ptr;
          ++replace_ptr;
        }

        ++results.replace_count;

        replace_ptr -= length_replace_str;
        find_ptr    -= length_find_str;
      }
    } else if (match_count == 0) {

      *final_string_ptr = *string_ptr;

    } else {
      find_ptr   -= match_count;

      string_ptr -= match_count;

      while (match_count > 0) {
        *final_string_ptr = *string_ptr;

        ++final_string_ptr;
        ++string_ptr;
        --match_count;
      }
    }

    ++final_string_ptr;
    ++string_ptr;
  }

  *final_string_ptr = '\0';

  printf("string_ptr: %s\n", results.string_ptr);
  printf("replace_count: %d\n", results.replace_count);

  return results;
}


/**********************************************************************************
 *                               - report_results -                               *
 *                                                                                *
 * prints results of running main program:                                        *
 *   'final_ptr' points to new string with replacements,                          *
 *   'num_replaced' indicates the number of replacement operations performed, and *
 *   'time_elapsed' indicates the CPU time elapsed across operations on input     *
 **********************************************************************************/
void report_results(char *final_ptr, int num_replaced, clock_t time_elapsed)
{
  /* printf("string_ptr:  %s\n",  string_ptr); */
  /* printf("find_ptr:    %s\n",    find_ptr); */
  /* printf("replace_ptr: %s\n", replace_ptr); */
}
