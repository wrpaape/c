#include <stdio.h>
char line[100]; /* line of data from the input */
int result;     /* the result of the calculations */
char operator;  /* operator the user specified */
int value;      /* value specified after the operator */


int main()
{
  result = 0; /* initialize the result */
  
  /* Loop forever (or until we hit the break statement)*/
  while (1) {
    printf("Result: %d\n", result);

    printf("Enter operator and number: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%c %d", &operator, &value);

    switch (operator) {
      case '+':
        result += value;
        break;
      case '-':
        result -= value;
        break;
      case '*':
        result *= value;
        break;
      case '/':
        if (value == 0) {
          printf("\e[31mdivide by zero error\e[0m\n");
        } else {
          result /= value;
        }
        break;
      case 'q':
        return 0;
        break;
      default:
        printf("\e[31munknown operator: %c\e[0m\n", operator);
    }
  }
}
