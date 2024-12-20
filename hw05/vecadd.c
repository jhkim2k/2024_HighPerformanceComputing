#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define N 120

main(int argc, char *argv[])
{
   int np, pid, local_N, dest, src, i, tag = 0;
   float A[N], B[N], C[N], *local_A, *local_B, *local_C;
   float num = 0.0;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if (pid == 0) {
      for (i = 0; i < N; i++) {
	 A[i] = i;
	 B[i] = N - i;
      }
   }

   local_N = N / np;
   
   local_A = (float*)malloc(sizeof(float)*local_N);
   local_B = (float*)malloc(sizeof(float)*local_N);
   local_C = (float*)malloc(sizeof(float)*local_N);
  
   // decomposition
   MPI_Scatter(A, local_N, MPI_FLOAT, local_A, local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
   MPI_Scatter(B, local_N, MPI_FLOAT, local_B, local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
      
   // addition
   for (i = 0; i < local_N; i++) {
      num = local_A[i] + local_B[i];
      local_C[i] += num;
   }
   
   // composition
   MPI_Gather(local_C, local_N, MPI_FLOAT, C, local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);

   if(pid == 0) {
      for (i = 0; i < N; i++) {
	 printf("%2.lf ", C[i]);
      }
      printf("\n");
   }

   free(local_A);
   free(local_B);
   free(local_C);
   MPI_Finalize();
}
