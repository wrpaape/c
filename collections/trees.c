#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 30 

typedef struct Node {
  char        word[BUFFER_SIZE - 1];         /* trimmed input */
  struct Node *less_than;    /* pointer to root of nodes alphabetically less than 'word' */
  struct Node *greater_than; /* pointer to root of nodes alphabetically greater than 'word' */
} Node;



Node *init_root(char *buffer, int buffer_size); /* sets root node to user input word */
void gets_next_word(char *buffer, int buffer_size); /* sets 'buffer' to user input word */
void insert_node(Node *node, const char *next_word);
void print_tree(Node *root);

const long unsigned int NODE_SIZE = sizeof(Node);

char buffer[BUFFER_SIZE]; /* input buffer */
/* Node *root; /1* root node *1/ */

int main(void)
{
   Node *root = init_root(buffer, BUFFER_SIZE);

/*   while (1) { */
/*     gets_next_word(buffer, BUFFER_SIZE); */

/*     if (buffer[0] == '\0') { */
/*       break; */
/*     } */

/*     insert_node(root, buffer); */

/*     print_tree(root); */
/*   } */

  return 0;
}

Node *init_root(char *buffer, int buffer_size)
{
  gets_next_word(buffer, buffer_size);

  if (buffer[0] == '\0') {
    exit(0);
  }

  Node *root = malloc(NODE_SIZE);

  strcpy(root -> word, buffer);

  printf("sizeof(*root) = %lu\n", sizeof(root));
  printf("NODE_SIZE = %lu\n", NODE_SIZE);
  printf("root -> word = %s\n", root -> word);
  fflush(stdout);

  print_tree(root);

  return root;
}

void gets_next_word(char *buffer, int buffer_size)
{
  printf("enter next word:\n  \e[5m>\e[25m ");

  fgets(buffer, BUFFER_SIZE, stdin);

  buffer[strcspn(buffer, "\r\n ")] = '\0'; /* trim input */
}


void insert_node(Node *node, const char *next_word)
{
  int comparison; /* result of string comparison with 'next_word' */

  comparison = strcmp(node -> word, next_word);

  if (comparison < 0) {
    printf("%s < %s\n", next_word, node -> word);
  } else if (comparison > 0) {
    printf("%s > %s\n", next_word, node -> word);
  } else {
    printf("%s == %s\n", next_word, node -> word);
  }
}


void print_tree(Node *root)
{
  printf("root -> word = %s", root -> word);
  fflush(stdout);
}
