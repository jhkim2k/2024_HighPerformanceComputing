#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

#define N 10
#define THREADS 10

__global__ void findmax(float *A, float *max)
{
   __shared__ float sdata[THREADS];
   int i = blockDim.x*blockIdx.x + threadIdx.x, half;

   sdata[threadIdx.x] = A[i];

   half = blockDim.x;
   for (unsigned int s = half; s > 0; s >>= 1) {
      
      if (threadIdx.x < s) {
	 if (sdata[threadIdx.x] < sdata[threadIdx.x + s]) {
	    sdata[threadIdx.x] = sdata[threadIdx.x + s];
	 }
      }

      __syncthreads();
   }

   if (threadIdx.x == 0) *max = sdata[0];
}

int main()
{
   float A[N], *A_d, max, *max_d;
   int i, dev;

   dim3 dimBlock(THREADS);
   dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x);

   dev = (getpwuid(getuid())->pw_name[3]-'0')%2? 1: 0;
   cudaSetDevice(dev);

   srand(1);
   for (i = 0; i < N; i++) {
      A[i] = rand() % 999;
      printf("%2.1f ", A[i]);
   }
   printf("\n");

   cudaMalloc((void **) &A_d, sizeof(float) * N);
   cudaMemcpy(A_d, A, sizeof(float) * N, cudaMemcpyHostToDevice);

   cudaMalloc((void **) &max_d, sizeof(float));

   findmax<<<dimGrid, dimBlock>>>(A_d, max_d);
   
   cudaMemcpy(&max, max_d, sizeof(float), cudaMemcpyDeviceToHost);

   printf("%f\n", max);

   cudaFree(A_d);
   cudaFree(max_d);
   exit(0);
}
