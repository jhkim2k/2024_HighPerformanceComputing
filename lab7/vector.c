#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
   int pid, np, flag, tag = 0, i, j;
   MPI_Status status;
   MPI_Datatype column_mpi_t;
   float A[5][5];

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   for (i = 0; i < 5; i++)
      for (j = 0; j < 5; j++)
	 A[i][j] = 0.0;

   if (pid == 0) {
      for (i = 0; i < 5; i++)
	 A[i][1] = 1.0;

      MPI_Type_vector(5, 1, 5, MPI_FLOAT, &column_mpi_t);
      MPI_Type_commit(&column_mpi_t);

      MPI_Send(&(A[0][1]), 1, column_mpi_t, 1, tag, MPI_COMM_WORLD);
   }

   if (pid == 1) {
      MPI_Type_vector(5, 1, 5, MPI_FLOAT, &column_mpi_t);
      MPI_Type_commit(&column_mpi_t);
      
      MPI_Recv(&(A[0][2]), 1, column_mpi_t, 0, tag, MPI_COMM_WORLD, &status);
      for (i = 0; i < 5; i++) {
	 for (j = 0; j < 5; j++)
	    printf("%2.1f ", A[i][j]);
	 printf("\n");
      }
   }
   MPI_Finalize();
}
