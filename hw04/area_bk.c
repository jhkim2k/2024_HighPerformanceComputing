#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define gflops(n, ms) (((n*18.0)/(ms/1.0e+3))/1.0e+9)

long millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return(1000*tv.tv_sec + tv.tv_usec/1000);
}

double f(double x)
{
   double temp;
   temp = x*x*x+1;
   return (9*x/(temp*temp));
}

int main(int argc, char *argv[])
{
   long N, i, nt, tid;
   double a, b, area, dx, x;
   long start, elapsed;

   if(argc != 3) {
      fprintf(stderr, "usage: %s #segments #threads\n", argv[0]);
      exit(1);
   }
   N = atoi(argv[1]);
   nt = atoi(argv[2]);

   start = millisecond();
   a = 0.0;
   b = 2.0;
   dx = (b-a)/N;

   omp_set_num_threads(nt);
   area = 0.0;
   #pragma omp parallel private(i, x, tid) shared(dx, nt, area)
   {
      tid = omp_get_thread_num();
      x = ((b-a)/(double)nt)*(double)tid;
      #pragma omp for reduction(+:area)
      for(i = 0; i < N; i++) {
	 area += 0.5*(f(x) + f(x+dx))*dx;
	 x += dx;
      }
   }
   elapsed = millisecond() - start;

   printf("area: %5.10lf\n", area);
   printf("elapsed time: %ld milliseconds", elapsed);
   printf(" (GFLOPS: %5.2f)\n", gflops(N, elapsed));

   exit(0);
}
