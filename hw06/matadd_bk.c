#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define M 12
#define N 10

float **malloc_2d();
void grid();

main(int argc, char *argv[])
{
   float A[M][N], B[M][N], C[M][N], **local_A, **local_B, **local_C;
   int np, inp, jnp, pid, local_M, local_N, *displs, *counts, i, j, n, tag = 0;
   MPI_Datatype vector_t;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   grid(M, N, np, pid, &inp, &jnp);
   if (pid == 0) printf("%dx%d processors are used\n", inp, jnp);

   local_M = M / inp;
   local_N = N / jnp;

   if (pid == 0) {
      for (i = 0; i < M; i++) {
	 for (j = 0; j < N; j++) {
	    A[i][j] = i * N;
	    B[i][j] = j;
	 }
      }
   }

   local_A = malloc_2d(local_M, local_N);
   local_B = malloc_2d(local_M, local_N);
   local_C = malloc_2d(local_M, local_N);

   // decompose
   displs = (int *)malloc(sizeof(int)*np);
   counts = (int *)malloc(sizeof(int)*np);

   n = 0;
   for (i = 0; i < inp; i++) {
      for (j = 0; j < jnp; j++) {
	 displs[n] = i * local_M * N + j * local_N;
	 counts[n++] = local_N;
      }
   }

   for (i = 0; i < local_M; i++) {
      MPI_Scatterv(&A[i][0], counts, displs, MPI_FLOAT, &local_A[i][0], local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
      MPI_Scatterv(&B[i][0], counts, displs, MPI_FLOAT, &local_B[i][0], local_N, MPI_FLOAT, 0, MPI_COMM_WORLD);
   }

   // local_C = local_A + local_B
   for (i = 0; i < local_M; i++) {
      for (j = 0; j < local_N; j++) {
	 local_C[i][j] = local_A[i][j] + local_B[i][j];
      }
   }

   //compose
   for (i = 0; i < local_M; i++) {
      MPI_Gatherv(&local_C[i][0], local_N, MPI_FLOAT, &C[i][0], counts, displs, MPI_FLOAT, 0, MPI_COMM_WORLD);
   }

   // check
   if (pid == 0) {
      for (i = 0; i < M; i++) {
	 for (j = 0; j < N; j++) {
	    printf("%3.0f", C[i][j]);
	 }
	 printf("\n");
      }
   }

   free(local_A);
   free(local_B);
   free(local_C);
   free(displs);
   free(counts);

   MPI_Finalize();
}
