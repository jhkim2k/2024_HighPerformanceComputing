#include <stdio.h>

#define N 12

#define A(i,j) A[i*N+j]

__global__ void gradual(int *A)
{
   int j = blockDim.y * blockIdx.y + threadIdx.y;
   int i = blockDim.x * blockIdx.x + threadIdx.x;

   if (j < N && i < N) {
      if (j <= i) {
	 A(i, j) = i;
      } else {
	 A(i, j) = j;
      }
   }
}

int main(int argc, char *argv[])
{
   int A[N][N], *A_d;
   int nt, i, j, dev;

   // GPU info
   cudaDeviceProp deviceProp;

   dev = 0;
   cudaSetDevice(dev);
   cudaGetDevice(&dev);
   cudaGetDeviceProperties(&deviceProp, dev);
   printf("Using Device %d: \"%s\"\n", dev, deviceProp.name);

   if (argc != 2) {
      fprintf(stderr, "usage: %s #threads\n", argv[0]);
      exit(0);
   }
   nt = atoi(argv[1]);

   dim3 dimBlock(nt, nt);
   dim3 dimGrid((N + dimBlock.x - 1)/dimBlock.x, (N + dimBlock.y - 1)/dimBlock.y);

   cudaMalloc((void **) &A_d, sizeof(float)*N*N);

   cudaMemcpy(A_d, A, N*N*sizeof(float), cudaMemcpyHostToDevice);
   
   gradual<<<dimGrid, dimBlock>>>(A_d);

   cudaMemcpy(A, A_d, N*N*sizeof(float), cudaMemcpyDeviceToHost);

   for (i = 0; i < N; i++) {
      for (j = 0; j < N; j++) {
	 printf("%3d ", A[i][j]);
      }
      printf("\n");
   }

   cudaFree(A_d);
   return 0;
}
