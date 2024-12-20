#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

#define MAX_THREADS 256

#define gflops(n, ms) (((n * 18.0) / (ms / 1.0e+3)) / 1.0e+9)

__device__ double f(double x)
{
   // 사다리꼴 적분을 위한 함수: f(x) = 9 * x / (x^3 + 1)^2
   double temp = x * x * x + 1;
   return (9 * x) / (temp * temp);
}

__global__ void area_kernel(double* local_area, long N, double a, double b)
{
   double dx = (b - a) / (double)N;
   long i = blockDim.x * blockIdx.x + threadIdx.x;
   extern __shared__ double sdata[];

   // 각 스레드는 x 좌표에서의 면적을 계산
   double x = a + i * dx;
   // sdata[threadIdx.x] = (i < N) ? 0.5 * (f(x) + f(x + dx)) * dx : 0.0;
   if (i < N) {
      sdata[threadIdx.x] = 0.5 * (f(x) + f(x + dx)) * dx;
   } else {
      sdata[threadIdx.x] = 0.0;
   }

   __syncthreads();

   // 공유 메모리를 사용하여 블록 내에서 부분 합을 계산
   for (int half = blockDim.x / 2; half > 0; half >>= 1) {
      if (threadIdx.x < half) {
	 sdata[threadIdx.x] += sdata[threadIdx.x + half];
      }
      __syncthreads();
   }

   // 블록의 첫 번째 스레드가 결과를 기록
   if (threadIdx.x == 0) {
      local_area[blockIdx.x] = sdata[0];
   }
}

int main(int argc, char* argv[])
{
   double *local_area, *local_area_d, area = 0.0, a = 0.0, b = 2.0;
   long N;
   int nt, i, smsize, dev;
   cudaEvent_t start, stop;
   float elapsed;
   cudaDeviceProp deviceProp;

   if (argc != 3) {
      fprintf(stderr, "usage: %s #segments #threads \n", argv[0]);
      exit(1);
   }
   N = atol(argv[1]);
   nt = atoi(argv[2]);

   if (nt > MAX_THREADS) {
      fprintf(stderr, "%d threads are used. \n", MAX_THREADS);
      nt = MAX_THREADS;
   }

   dim3 dimBlock(nt);
   dim3 dimGrid((N + dimBlock.x - 1) / dimBlock.x);
   smsize = sizeof(double) * nt;

   // 현재 사용자와 디바이스 ID를 바탕으로 GPU 선택
   dev = (getpwuid(getuid())->pw_name[3] - '0') % 2 ? 1 : 0;
   cudaSetDevice(dev);
   cudaGetDeviceProperties(&deviceProp, dev);
   printf("Device(%d) used: \"%s\"\n", dev, deviceProp.name);

   local_area = (double*)malloc(sizeof(double) * dimGrid.x);
   cudaMalloc((void**)&local_area_d, sizeof(double) * dimGrid.x);

   // 타이머 시작: 계산 시간 측정
   cudaEventCreate(&start);
   cudaEventCreate(&stop);
   cudaEventRecord(start, 0);

   // GPU 커널 호출
   area_kernel<<<dimGrid, dimBlock, smsize>>>(local_area_d, N, a, b);

   // GPU에서 CPU로 결과 복사
   cudaMemcpy(local_area, local_area_d, sizeof(double) * dimGrid.x, cudaMemcpyDeviceToHost);

   // 타이머 종료: 시간 측정 완료
   cudaEventRecord(stop, 0);
   cudaEventSynchronize(stop);
   cudaEventElapsedTime(&elapsed, start, stop);

   // GPU에서 계산된 부분 합을 CPU에서 최종 합산
   for (i = 0; i < dimGrid.x; i++) {
      area += local_area[i];
   }

   printf("area: %5.5lf\n", area);
   printf("elapsed time: %5.2f milliseconds", elapsed);
   printf(" (GFLOPS: %5.2f)\n", gflops(N, elapsed));

   free(local_area);
   cudaFree(local_area_d);
   exit(0);
}

