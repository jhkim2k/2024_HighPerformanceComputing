#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define gflops(n, ms) (((n*18.0)/(ms/1.0e+6))/1.0e+9)

long millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return ((1000000*tv.tv_sec) + (tv.tv_usec));
}

double f(double x)
{
   double temp;
   temp = x*x*x+1;
   return (9*x/(temp*temp));
}

int main(int argc, char *argv[])
{
   long N, i;
   double a, b, area_seq, area_par, dx, x;
   long start = 0, elapsed_seq = 0, elapsed_par = 0;
   long num_threads;

   if(argc != 3) {
      fprintf(stderr, "usage: %s number_of_segments\n", argv[0]);
      exit(1);
   }

   N = atoi(argv[1]);
   num_threads = atoi(argv[2]);
   
   a = 0.0;
   b = 2.0;
   dx = (b-a)/(double)N;

   start = millisecond();
   area_seq = 0.0;
   x = a;
   for(i = 0; i < N; i++) {
      area_seq += 0.5*(f(x)+f(x+dx))*dx;
      x += dx;
   }
   elapsed_seq = millisecond() - start;
   
   start = millisecond();
   area_par = 0.0;
   
   #pragma omp parallel for private(i, x) shared(a, b, dx, N) reduction(+:area_par) num_threads(num_threads)
   for(i = 0; i < N; i++) {
      x = a + i * dx;
      area_par += 0.5*(f(x)+f(x+dx))*dx;
   }
   elapsed_par = millisecond() - start;
   
   printf("Sequential\n");
   printf("area: %5.10lf\n", area_seq);
   printf("elapsed time: %ld milliseconds ", elapsed_seq/1000);
   printf("(GFLOPS: %5.2f)\n", gflops(N, elapsed_seq));

   printf("\nParallel\n");
   printf("area: %5.10lf\n", area_par);
   printf("elapsed time: %ld milliseconds ", elapsed_par/1000);
   printf("(GFLOPS: %5.2f)\n", gflops(N, elapsed_par));

   exit(0);
}
