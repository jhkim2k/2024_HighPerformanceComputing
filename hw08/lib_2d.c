#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define TYPE int

TYPE **malloc_2d(int row, int col)
{
   TYPE **A, *ptr;
   int len, i;

   len = sizeof(TYPE *)*row + sizeof(TYPE)*col*row;
   A = (TYPE **)malloc(len);
   ptr = (TYPE *)(A + row);
   for(i = 0; i < row; i++)
      A[i] = (ptr + col*i);
   return A;
}

// find optimal numbers of grid processors
void grid(int M, int N, int np, int pid, int *inp, int *jnp)
{
   int i, ip, jp;

   ip = (int)round(sqrt(((float)M/(float)N)*(float)np));
   if (ip > np) ip = np;
   for (i=ip; i<=np; i++) {
      jp = (int)round(np/ip);
      if (ip*jp != np) ip = i;
   }

   if (((float)M/(float)ip) != M/ip) {
      if (pid == 0) fprintf(stderr, "err: M(%d) not divisible by %d\n", M, ip);
      MPI_Finalize();
      exit(1);
   }
   if (((float)N/(float)jp) != N/jp) {
      if (pid == 0) fprintf(stderr, "err: N(%d) not divisible by %d\n", N, jp);
      MPI_Finalize();
      exit(2);
   }
   *inp = ip; *jnp = jp;
   return;
}
