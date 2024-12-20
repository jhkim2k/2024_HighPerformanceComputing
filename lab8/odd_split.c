#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
   int pid, np, flag, tag = 0, i;
   MPI_Status status;

   MPI_Comm comm_odd;
   int data;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   data = pid;
   
   flag = (pid % 2) + 1;
   MPI_Comm_split(MPI_COMM_WORLD, flag, pid, &comm_odd);

   if (pid % 2) {
      MPI_Bcast(&data, 1, MPI_INT, 1, comm_odd);
      printf("%d %d\n", pid, data);
   }

   MPI_Finalize();
   exit(0);
}
