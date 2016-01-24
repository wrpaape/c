#include <stdio.h>
/* #include <stdlib.h> */
#include <string.h>

#define BUFFER_SIZE 30 

char buffer[BUFFER_SIZE]; /* input buffer */

typedef struct Node {
  char        word[BUFFER_SIZE]; /* trimmed input */
  struct Node *less_than;        /* pointer to root of nodes less than '*random' */
  struct Node *greater_than;     /* pointer to root of nodes less than '*random' */
} Node;


int main(void)
{
  Node *node;

  printf("enter next word:\n  \e[5m>\e[25m ");

  fgets(node -> word, BUFFER_SIZE, stdin);

  (node -> word)[strlen(node -> word) - 1] = '\0'; /* trim input */


  printf("node -> word = %s", node -> word);
  
  return 0;
}
