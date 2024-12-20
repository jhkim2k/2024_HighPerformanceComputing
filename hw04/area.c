#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mpi.h"

#define gflops(n, ms) (((n*18.0)/(ms/1.0e+3))/1.0e+9)

long millisecond()
{
   struct timeval tv;
   gettimeofday(&tv, NULL);
   return(1000*tv.tv_sec + tv.tv_usec/1000);
}

double f(double x)
{
   double temp;
   temp = x*x*x+1;
   return (9*x/(temp*temp));
}

int main(int argc, char *argv[])
{
   long N, i, np, pid, dest;
   long tag = 0;
   double a, b, area, dx, x;
   double local_N, local_a, local_b, local_area;
   long start, elapsed;
   int tem_np, tem_pid;
   MPI_Status status;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &tem_np);
   np = (long)tem_np;
   MPI_Comm_rank(MPI_COMM_WORLD, &tem_pid);
   pid = (long)tem_pid;

   if(argc != 2) {
      fprintf(stderr, "usage: %s N\n", argv[0]);
      exit(1);
   }
   N = atol(argv[1]);

   
   start = millisecond();
   a = 0.0;
   b = 2.0;
   dx = (b-a)/(double)N;

   local_N = N / np;
   local_a = a + pid * local_N * dx;
   local_b = local_a + local_N * dx;

   local_area = 0.0;
   x = local_a;
   for(i = 0; i < local_N; i++) {
      local_area += 0.5*(f(x) + f(x+dx))*dx;
      x += dx;
   }

   if(pid == 0) {
      //start = millisecond();
      area = local_area;
      double recv_area;
      for(dest = 1; dest < np; dest++) {
	 MPI_Recv(&recv_area, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD, &status);
	 area += recv_area;
      }
      elapsed = millisecond() - start;

      printf("area: %5.10lf\n", area);
      printf("elapsed time: %ld milliseconds", elapsed);
      printf(" (GFLOPS: %5.2f)\n", gflops(N, elapsed));
   } else {
      MPI_Send(&local_area, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
   }

   MPI_Finalize();

   exit(0);
}
