#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double f(double x)
{
   if(x == 0.0)
   {
      return 0.0;
   }
   return x / ((x*x+2) * (x*x+2) * (x*x+2));
}

double trapezoidal(int N)
{
   double a = 0.0;
   double b = 2.0;
   double sum = 0.0;
   double dx = (b - a) / N;
   double x = a;
   int i = 0;

   for(i = 0; i < N; i++)
   {
      sum += 0.5 * (f(x) + f(x + dx)) * dx;
      x += dx;
   }
   
   return sum;
}

int main(int argc, char *argv[])
{
   if(argc != 2)
   {
      printf("argument error\n");
      exit(1);
   }

   long N = atoi(argv[1]);
   double area;

   area = trapezoidal(N);

   printf("total area: %lf\n", area);

   return 0;
}
