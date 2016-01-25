/************************************************************************************
 *                             PREPROCESSOR DIRECTIVES                              *
 ************************************************************************************/
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
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

typedef struct Boolean {
  unsigned int is_true : 1;
} Boolean;

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
void print_next(int nodec, Node *nodev[], int padding);



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


void populate_tree(Node *root, char *buffer, int buffer_size)
{
  Boolean boolean;

  while (1) {
    printf("******************************\n\n");

    if (boolean.is_true) {
      printf("\e[7m");
    }

    gets_next_word(buffer, BUFFER_SIZE);

    if (buffer[0] == '\0') {
      return;
    }

    printf("\n");

    insert_next(root, buffer, 0);

    printf("\e[0m\n******************************\n");

    boolean.is_true = ~boolean.is_true;
  }
}


void print_tree(Node *root)
{
  if (root == NULL) {
    return;
  }

  print_tree(root -> less_than);
  printf("%s\n", root -> word);
  print_tree(root -> greater_than);
  /* struct winsize window; */
  /* ioctl(STDOUT_FILENO, TIOCGWINSZ, &window); */

  /* print_next(root, window.ws_col); */
  /* printf("root -> word = %s\n", root -> word); */
  /* printf("terminal width = %i\n", window.ws_col); */
}

/************************************************************************************
 *                                HELPER FUNCTIONS                                  *
 ************************************************************************************/
void gets_next_word(char *buffer, int buffer_size)
{
  printf("enter next word:\n  \e[5m>\e[25m ");

  fgets(buffer, BUFFER_SIZE, stdin);

  buffer[strcspn(buffer, "\r\n ")] = '\0'; /* trim trailing newline from input */
}


void insert_next(Node *node, const char *next_word, int level)
{
  int comparison;       /* integer result of alphabetical comparison 'strcomp' */
  Node **next_node_ptr; /* pointer to nodes either greater or less than 'node' */

  comparison = strcmp(next_word, node -> word);

  if (comparison < 0) {
    printf("next_word (%s) < node -> word (%s)\n", next_word, node -> word);
    next_node_ptr = &(node -> less_than);

  } else if (comparison > 0) {
    printf("next_word (%s) > node -> word (%s)\n", next_word, node -> word);
    next_node_ptr = &(node -> greater_than);

  } else {
    printf("next_word (%s) == node -> word (%s)\nskipping duplicate!\n", next_word, node -> word);
    return;
  }

  if (*next_node_ptr == NULL) {
    printf("allocating memory...\n");

    *next_node_ptr = malloc(NODE_SIZE);

    if (*next_node_ptr == NULL) {
      printf("\n\e[5m\e[31mERROR: OUT OF MEMORY\e[0m\n");
      exit(8);
    }

    strcpy((*next_node_ptr) -> word, next_word);
    (*next_node_ptr) -> less_than    = NULL;
    (*next_node_ptr) -> greater_than = NULL;

    printf("insert successful (%i levels deep)!\n", level);
    return;
  }

  insert_next(*next_node_ptr, next_word, ++level);
}

void print_next(int nodec, Node *nodev[], int padding)
{

}
