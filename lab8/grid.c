#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
   int np, pid, inp, jnp, i, j, k, pid_from, *buf;

   int dim_sizes[2], wrap_around[2];
   int direct, shift, source, dest, reorder, tag = 0;
   MPI_Comm grid_comm;
   MPI_Status status;

   if (argc != 3) {
      printf("usage %s direct shift\n", argv[0]);
      exit(1);
   }
   direct = atoi(argv[1]);
   shift = atoi(argv[2]);

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &pid);
   MPI_Comm_size(MPI_COMM_WORLD, &np);

   inp = sqrt(np);

   if (np != inp * inp) {
      if (pid == 0) fprintf(stderr, "#proc is not a square number of 2\n");
      MPI_Finalize();
      exit(0);
   }

   jnp = np / inp;
   dim_sizes[0] = inp;
   dim_sizes[1] = jnp;
   wrap_around[0] = wrap_around[1] = 1;

   MPI_Cart_create(MPI_COMM_WORLD, 2, dim_sizes, wrap_around, reorder, &grid_comm);
   MPI_Cart_shift(grid_comm, direct, shift, &source, &dest);

   MPI_Sendrecv(&pid, 1, MPI_INT, dest, tag, &pid_from, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

   if (pid == 0) {
      buf = (int *)malloc(sizeof(int)*np);
   }

   MPI_Gather(&pid_from, 1, MPI_INT, buf, 1, MPI_INT, 0, MPI_COMM_WORLD);

   if (pid == 0) {
      k = 0;
      for (i = 0; i < inp; i++) {
	 for (j = 0; j < jnp; j++)
	    printf("%2d ", buf[k++]);
	 printf("\n");
      }
      free(buf);
   }

   MPI_Finalize();
   exit(0);
}
