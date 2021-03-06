#include <stdio.h>

char input[100];  /* input string degrees celsius */
float celsius;    /* degrees celsius */
float fahrenheit; /* input string degrees fahrenheit */

int main()
{
  printf("input °C: ");

  fgets(input, sizeof(input), stdin);
  sscanf(input, "%f", &celsius);

  fahrenheit = 9.0 * celsius / 5.0 + 32.0;

  printf("\e[31m%f °C is %f °F\n", celsius, fahrenheit);
  
  return 0;
}
