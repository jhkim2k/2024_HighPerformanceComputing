#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mpi.h"

#define N 12000

int next = 1;
int rand2(int a, int c)
{
   return (next = next * a + c);
}

void srand2(int seed)
{
   next = seed;
}

void random_p(int n, int a, int c, int arr[n])
{
   int i;

   for (i = 1; i < n; i++)
      arr[i] = rand2(a, c);
}

int main(int argc, char *argv[])
{
   int X[N], *local_X, np, pid, local_N, i, min, temp;
   int a, c, A, C;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   local_N = N / np;
   srand2(1);
   a = 1103515245;
   c = 12345;
   A = 1; C = 0;
   for (i = 0; i < np; i++){
      A = (a * A);
      C = (a * C + c);
   }

   local_X = malloc(sizeof(int)*local_N);

   srand2(1);
   for (i = 0; i < pid; i++)
      local_X[0] = rand2(a, c);

   random_p(local_N, A, C, local_X);

   min = INT_MAX;

   for (i = 0; i < local_N; i++) {
      if (local_X[i] < min) {
	 min = local_X[i];
      }
   }

   MPI_Reduce(&min, &temp, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

   if (pid == 0) printf("%d\n", min);

   MPI_Finalize();
   free(local_X);
   return 0;
}
