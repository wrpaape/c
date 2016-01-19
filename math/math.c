#include <stdio.h>
#include <math.h>
float square_root;
float exponent;

int main()
{
  square_root = sqrt(5);
  exponent = pow(5, 3);

  printf("%f\n", square_root);
  printf("%f\n", exponent);
  
  return 0;
}
