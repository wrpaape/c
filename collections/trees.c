/************************************************************************************
 *                             PREPROCESSOR DIRECTIVES                              *
 ************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 30 

/************************************************************************************
 *                               INITIAL DECLARATIONS                               *
 ************************************************************************************/
typedef struct Node {
  char word[BUFFER_SIZE - 1]; /* trimmed input */
  struct Node *less_than;     /* pointer to lesser nodes */
  struct Node *greater_than;  /* pointer to greater nodes */
} Node;                       /* models a node in a generic tree of sorted strings */

const unsigned int NODE_SIZE = sizeof(Node); /* used with 'malloc' to initialize new nodes */

/************************************************************************************
 *                               FUNCTION PROTOTYPES                                *
 ************************************************************************************/
/* top level */
void init_root(Node *root, char *buffer, int buffer_size);
void populate_tree(Node *root, char *buffer, int buffer_size);
void print_tree(Node *root);
/* helpers */
void gets_next_word(char *buffer, int buffer_size);
void insert_next(Node *node, const char *next_word, int level);



/************************************************************************************
 *                                  MAIN FUNCTION                                   *
 ************************************************************************************/
int main(void)
{
  Node *root;               /* root node */
  char buffer[BUFFER_SIZE]; /* input buffer */

  init_root(root, buffer, BUFFER_SIZE);

  populate_tree(root, buffer, BUFFER_SIZE);

  print_tree(root);

  return 0;
}

/************************************************************************************
 *                               TOP LEVEL FUNCTIONS                                *
 ************************************************************************************/

void init_root(Node *root, char *buffer, int buffer_size)
{
  gets_next_word(buffer, buffer_size);

  if (buffer[0] == '\0') {
    exit(0);
  }

  strcpy(root -> word, buffer);
  root -> less_than    = NULL;
  root -> greater_than = NULL;
}



/* void populate_tree(Node *root, char *buffer, int buffer_size) */
void populate_tree(Node *root, char *buffer, int buffer_size)
{

  while (1) {
    gets_next_word(buffer, BUFFER_SIZE);

    if (buffer[0] == '\0') {
      return;
    }

    printf("root = %p\n", root);
    printf("root -> word = %s\n", root -> word);
    printf("root -> less_than = %p\n", root -> less_than);
    printf("root -> greater_than = %p\n", root -> greater_than);
    printf("***********************************************\n");
    fflush(stdout);
    insert_next(root, buffer, 0);
    printf("***********************************************\n");
    printf("root = %p\n", root);
    printf("root -> word = %s\n", root -> word);
    printf("root -> less_than = %p\n", root -> less_than);
    printf("root -> greater_than = %p\n", root -> greater_than);
    fflush(stdout);
  }
}


void print_tree(Node *root)
{
  printf("root -> word = %s", root -> word);
}

/************************************************************************************
 *                                HELPER FUNCTIONS                                  *
 ************************************************************************************/

void gets_next_word(char *buffer, int buffer_size)
{
  printf("enter next word:\n  \e[5m>\e[25m ");

  fgets(buffer, BUFFER_SIZE, stdin);

  buffer[strcspn(buffer, "\r\n ")] = '\0'; /* trim input */
}


void insert_next(Node *node, const char *next_word, int level)
{
  if (node == NULL) {
    printf("allocating memory...\n");

    node = malloc(NODE_SIZE);

    if (node == NULL) {
      printf("\n\e[5m\e[31mERROR OUT OF MEMORY\e[0m\n");
      exit(8);
    }

    strcpy(node -> word, next_word);
    node -> less_than    = NULL;
    node -> greater_than = NULL;

    printf("insert successful (%i levels deep)!\n", level);

    return;
  }

  int comparison = strcmp(next_word, node -> word); /* alphabetical comparison */

  if (comparison < 0) {
    printf("next_word (%s) < node -> word (%s)\n", next_word, node -> word);
    insert_next(node -> less_than, next_word, ++level);

  } else if (comparison > 0) {
    printf("next_word (%s) > node -> word (%s)\n", next_word, node -> word);
    insert_next(node -> greater_than, next_word, ++level);

  } else {
    printf("next_word (%s) == node -> word (%s)\n", next_word, node -> word);
    return;
  }
}
  /* printf("sizeof(*root) = %lu\n", sizeof(*root)); */
  /* printf("NODE_SIZE = %lu\n", NODE_SIZE); */
  /* printf("root -> word = %s\n", root -> word); */
  /* fflush(stdout); */

