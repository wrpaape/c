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
  /* Node *root = NULL;        /1* root node *1/ */
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

    printf("root = %p", root);
    fflush(stdout);
    insert_next(root, buffer, 0);
    printf("root = %p", root);
    printf("root -> word = %s", root -> word);
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


/* void insert_next(Node *node, const char *next_word) */
/* { */
/*   printf("comparing next_word (%s) with node -> word (%s)...\n", next_word, node -> word); */
/*   int comparison = strcmp(next_word, node -> word); /1* alphabetical comparison *1/ */

/*   printf("comparison = %i\n", comparison); */

/*   if (comparison == 0) { */
/*     return; /1* duplicate word, no need to create a new node *1/ */
/*   } */

/*   Node *next_node = (comparison < 0) ? (node -> less_than) : (node -> greater_than); */
/*   /1* Node *next_node = (comparison > 0) ? (node -> greater_than) : (node -> less_than); *1/ */



/*     printf("NULL      = %p\n", NULL); */
/*     printf("next_node = %p\n", next_node); */
/*     /1* puts(NULL); *1/ */
/*     /1* puts(&*next_node); *1/ */
/*     fflush(stdout); */

/*   if (next_node == NULL) { */
/*     printf("allocating memory...\n"); */

/*     next_node = malloc(NODE_SIZE); */

/*     if (next_node == NULL) { */
/*       printf("\n\e[5m\e[31mERROR MEMORY LEAK\e[0m\n"); */
/*       exit(8); */
/*     } */

/*     strcpy(next_node -> word, next_word); */
/*     next_node -> less_than    = NULL; */
/*     next_node -> greater_than = NULL; */

/*     printf("insert successful!\n"); */

/*   } else { */
/*     /1* printf("sizeof(*next_node) = %lu\n", sizeof(*next_node)); *1/ */
/*     /1* printf("next_node -> word = %s\n", next_node -> word); *1/ */
/*     insert_next(next_node, next_word); */
/*   } */
/* } */

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

