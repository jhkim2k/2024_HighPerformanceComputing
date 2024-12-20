#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000
#define range(n) ((unsigned int)(int)((n)/65535)%32768)

int next = 1;

int rand2(int a, int c)
{
   return (next = next * a + c);
}

void srand2(int seed)
{
   next = seed;
}

int main(int argc, char *argv[])
{
   int X[N], Y[N];
   int nt, tid, local_N, i, j;
   int sum_X = 0, sum_Y = 0;
   int a, c, A, C;

   if(argc != 2) {
      printf("usage: %s #threads\n", argv[0]);
      exit(1);
   }

   nt = atoi(argv[1]);

   srand2(1);
   a = 1103515245;
   c = 12345;
   for(i = 0; i < N; i++) {
      X[i] = rand2(a, c);
   }
   local_N = N / nt;

   srand2(1);
   for (i = 0; i < N; i += local_N)
      Y[i] = rand2(a, c);

   A = 1; C = 0;
   for(i = 0; i < nt; i++) {
      A = (a * A);
      C = (a * C + c);
   }

   #pragma omp parallel for private(i) num_threads(nt) reduction(+:sum_X, sum_Y)
   for(i = 0; i < N; i++) {
      if(!(i % local_N)) srand2(Y[i]);
      else Y[i] = rand2(A, C);
   }

   #pragma omp parallel for reduction(+:sum_X, sum_Y) num_threads(nt)
   for (i = 0; i < N; i++) {
      sum_X += range(X[i]);
      sum_Y += range(Y[i]);
   }

   printf("Sum of X: %d\n", sum_X);
   printf("Sum of Y: %d\n", sum_Y);

   if (sum_X == sum_Y) {
      printf("Equal.\n");
   } else {
      printf("Different.\n");
   }
   
   exit(0);
}
