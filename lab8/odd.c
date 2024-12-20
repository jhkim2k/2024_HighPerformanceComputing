#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
   int pid, np, tag = 0, i;
   MPI_Status status;

   MPI_Group group_world, group_odd;
   MPI_Comm comm_odd;
   int *newrank;
   int data;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   data = 10 + pid;
   newrank = (int *)(malloc(np / 2 * sizeof(int)));
   for (i = 0; i < np / 2; i++) {
      newrank[i] = (i * 2) + 1;
   }

   MPI_Comm_group(MPI_COMM_WORLD, &group_world);
   MPI_Group_incl(group_world, np / 2, newrank, &group_odd);
   MPI_Comm_create(MPI_COMM_WORLD, group_odd, &comm_odd);

   if (pid%2) {
      MPI_Bcast(&data, 1, MPI_INT, 2, comm_odd);
      printf("%d %d\n", pid, data);
   }

   free(newrank);
   MPI_Finalize();
   exit(0);
}
