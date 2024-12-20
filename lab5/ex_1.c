#include <stdio.h>
#include "mpi.h"

main(int argc, char *argv[])
{
   char processor_name[80];
   int pid, name_len;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &pid);

   MPI_Get_processor_name(processor_name, &name_len);
   printf("%s, rank %d\n", processor_name, pid);

   MPI_Finalize();
}
