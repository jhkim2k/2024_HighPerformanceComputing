#include <stdio.h>
#include "mpi.h"

main(int argc, char *argv[])
{
   int np, pid, tag = 0;
   float data;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &np);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   if (pid == 0) data = 100.0;
   
   MPI_Bcast(&data, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

   printf("%f\n", pid + data);
   MPI_Finalize();
}
