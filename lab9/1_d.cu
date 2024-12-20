#include <stdio.h>
#define X 12
#define THREAD_X 4
__global__ void index(int *A)
{
   int i = blockDim.x*blockIdx.x+threadIdx.x;
   //(1) 
   A[i] = 123;
   //(2) 
   A[i] = i;
   //(3) 
   A[i] = gridDim.x;
   //(4) 
   A[i] = blockDim.x;
   //(5) 
   A[i] = threadIdx.x;
}
int main()
{
   int A[X], *A_d, i;
   dim3 dimBlock(THREAD_X);
   dim3 dimGrid(X/ dimBlock.x);
   
   cudaSetDevice(0); // or 1
   cudaMalloc((void **)&A_d, sizeof(int)*X);
   for (i=0; i<X; i++)
      A[i] = 1;
   
   cudaMemcpy(A_d, A, sizeof(int)*X, cudaMemcpyHostToDevice);
   
   index<<<dimGrid, dimBlock>>>(A_d);
	
   cudaMemcpy(A, A_d, sizeof(int)*X, cudaMemcpyDeviceToHost);

   for (i=0; i<X; i++)
      printf("%d ", A[i]);
   printf("\n");
   cudaFree(A_d);
}
