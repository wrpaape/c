#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_SIZE 50
/* #define NUM_NAMES 4 */


struct List {
  char data[DATA_SIZE];  /* element's data */
  struct List *next_ptr; /* pointer to next element in list */
};


int main(void)
{
  const char  *NAMES[]     = {"Bobby", "Sam", "Joey", "Tom"};
  size_t      NUM_NAMES    = sizeof(NAMES) / sizeof(*NAMES);
  const int   EL_SIZE      = sizeof(struct List);
  struct List *head_ptr    = NULL;
  struct List *next_el_ptr = NULL;

  /* printf("%zu\n", NUM_NAMES); */
  /* NAMES     = {"Bob", "Sam", "Jim", "Tom"}; */
  /* NUM_NAMES = sizeof(NAMES); */
  /* EL_SIZE   = sizeof(struct List); */


  for (int i = 0; i < NUM_NAMES; ++i) {
    next_el_ptr = malloc(EL_SIZE);
    
    strcpy(next_el_ptr -> data, NAMES[i]);

    next_el_ptr -> next_ptr = head_ptr;

    head_ptr = next_el_ptr;
  }

  while (head_ptr != NULL) {
    printf("head: %s\n", head_ptr -> data);

    head_ptr = head_ptr -> next_ptr;
  } 

  return 0;
}
