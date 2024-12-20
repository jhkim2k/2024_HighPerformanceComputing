#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

double millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return(1000.0*tv.tv_sec + tv.tv_usec/1000.0);
}

double f(double x)
{
   return (9*x) / ((x*x*x+1) * (x*x*x+1));
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

   int num = atoi(argv[1]);
   double area;
   double start, stop;
   double elapsed_time = 0.0;
   double gflops = 0.0;

   start = millisecond();
   area = trapezoidal(num);
   stop = millisecond();
   
   elapsed_time = (double)(stop - start);
   gflops = ((num * 24) + 2) / (elapsed_time * 1e6); 

   printf("total area: %lf\n", area);
   printf("elapsed time: %lf milliseconds\n", elapsed_time);
   printf("GFLOPS: %lf\n", gflops);

   return 0;
}
