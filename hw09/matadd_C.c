#include <stdio.h>
#include <stdlib.h>
#define M 12
#define N 10

int main()
{
   float A[M][N], B[M][N], C[M][N];
   int i, j;
   // initializaton of A and B
   for (i=0; i<M; i++)
      for (j=0; j<N; j++) {
	 A[i][j] = M - i + 1;
	 B[i][j] = N + j;
      }

   // add two matrixes
   for (i=0; i<M; i++)
      for (j=0; j<N; j++)
	 C[i][j] = A[i][j] + B[i][j];
	 
   // print the results
   for (i=0; i<M; i++) {
      for (j=0; j<N; j++)
	 printf("%3.0f ", C[i][j]);
      printf("\n");
   }

   exit(0);
}
