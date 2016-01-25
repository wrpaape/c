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
void print_tree(Node root);
/* helpers */
void gets_next_word(char *buffer, int buffer_size);
void insert_next(Node *node, const char *next_word);



/************************************************************************************
 *                                  MAIN FUNCTION                                   *
 ************************************************************************************/
int main(void)
{
  Node root;                /* root node */
  char buffer[BUFFER_SIZE]; /* input buffer */

  init_root(&root, buffer, BUFFER_SIZE);

  populate_tree(&root, buffer, BUFFER_SIZE);

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
}



void populate_tree(Node *root, char *buffer, int buffer_size)
{
  while (1) {
    gets_next_word(buffer, BUFFER_SIZE);

    if (buffer[0] == '\0') {
      return;
    }

    insert_next(root, buffer);
  }
}


void print_tree(Node root)
{
  printf("root.word = %s", root.word);
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



void insert_next(Node *node, const char *next_word)
{
  printf("comparing next_word (%s) with node -> word (%s)...\n", next_word, node -> word);
  int comparison = strcmp(next_word, node -> word); /* alphabetical comparison */

  if (comparison == 0) {
    return; /* duplicate word, no need to create a new node */
  }

  Node *next_node = (comparison < 0) ? (node -> less_than) : (node -> greater_than);

  if (next_node == NULL) {
    printf("allocating memory...\n");
    fflush(stdout);

    next_node = malloc(NODE_SIZE);

    strcpy(next_node -> word, next_word);

    printf("insert successful!\n");

  } else {
    insert_next(next_node, next_word);
  }
}

  /* printf("sizeof(*root) = %lu\n", sizeof(*root)); */
  /* printf("NODE_SIZE = %lu\n", NODE_SIZE); */
  /* printf("root -> word = %s\n", root -> word); */
  /* fflush(stdout); */

