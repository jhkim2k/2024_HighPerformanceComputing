#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define M 12
#define N 10

int **malloc_2d();
void grid();

int main(int argc, char *argv[])
{
   int A[M][N] = {[0 ... M-1][0 ... N-1] = 0}, **local_A;
   int B[M][N] = {[0 ... M-1][0 ... N-1] = 0};
   int np, inp, jnp, pid, i, j, tag = 0;
   int dim_sizes[2], coord[2], wrap_around[2], reorder = 0;
   int local_M, local_N, x, y;
   MPI_Comm grid_comm;
   MPI_Datatype vector_t;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   grid(M, N, np, pid, &inp, &jnp);
   if (pid == 0) printf("%dx%d processors are used\n", inp, jnp);

   if (pid == 0) {
      for (j = 0; j < N; j++) {
	 B[0][j] = j;
	 B[M-1][j] = M-1+j;
      }
      for (i = 0; i < M; i++) {
	 B[i][0] = i;
	 B[i][N-1] = N-1+i;
      }

      printf("1 processor is used\n");
      for (i = 0; i < M; i++) {
	 for (j = 0; j < N; j++)
	    printf("%4d ", B[i][j]);
	 printf("\n");
      }
   }

   dim_sizes[0] = inp;
   dim_sizes[1] = jnp;
   wrap_around[0] = 0;
   wrap_around[1] = 0;

   MPI_Cart_create(MPI_COMM_WORLD, 2, dim_sizes, wrap_around, reorder, &grid_comm);
   MPI_Cart_coords(grid_comm, pid, 2, coord);

   local_M = M / inp;
   local_N = N / jnp;

   MPI_Type_vector(local_M, local_N, N, MPI_INT, &vector_t);
   MPI_Type_commit(&vector_t);

   local_A = malloc_2d(local_M, local_N);

   for (i = 0; i < local_M; i++) {
      for (j = 0; j < local_N; j++) {
	 local_A[i][j] = 0;
      }
   }

   
   if (coord[0] == 0) { // Top
      for (j = 0; j < local_N; j++)
	 local_A[0][j] = coord[1] * local_N + j;
   }
   if (coord[0] == inp - 1) { // Bottom
      for (j = 0; j < local_N; j++)
	 local_A[local_M - 1][j] = (M - 1) + coord[1] * local_N + j;
   }
   if (coord[1] == 0) { // Leftmost
      for (i = 0; i < local_M; i++)
	 local_A[i][0] = coord[0] * local_M + i;
   }
   if (coord[1] == jnp - 1) { // Rightmost
      for (i = 0; i < local_M; i++)
	 local_A[i][local_N - 1] = (N - 1) + coord[0] * local_M + i;
   }
  
   for (i = 0; i < local_M; i++) {
      for (j = 0; j < local_N; j++) {
	 printf("%4d ", local_A[i][j]);
      }
   }
   printf("\n\n");


   if (pid == 0) {
      for (i = 0; i < local_M; i++) {
	 for (j = 0; j < local_N; j++) {
	    A[i][j] = local_A[i][j];
	 }
      }
      for (i = 1; i < np; i++) {
	 int source_coords[2];
	 MPI_Cart_coords(grid_comm, i, 2, source_coords);
	 
	 int start_row = source_coords[0] * local_M;
	 int start_col = source_coords[1] * local_N;
	 
	 MPI_Datatype subarray_t;
	 int sizes[2] = {M, N};               // Global array size
	 int subsizes[2] = {local_M, local_N}; // Local block size
	 int starts[2] = {start_row, start_col}; // Start indices
	 
	 MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_INT, &subarray_t);
	 MPI_Type_commit(&subarray_t);
	 MPI_Recv(&A[0][0], 1, subarray_t, i, tag, MPI_COMM_WORLD, &status);
      }
   }
   else {
      MPI_Send(&(local_A[0][0]), 1, vector_t, 0, tag, MPI_COMM_WORLD);
   }

   if (pid == 0) {
      printf("\n%dx%d processors are used\n", inp, jnp);
      for (i = 0; i < M; i++) {
	 for (j = 0; j < N; j++) {
	    printf("%4d ", A[i][j]);
	 }
	 printf("\n");
      }
   }

   free(local_A);
   MPI_Finalize();
   exit(0);
}
