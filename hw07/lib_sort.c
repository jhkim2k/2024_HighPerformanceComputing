#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int next = 1;

void srand2(int seed)
{
   next = seed;
}

int rand2(int a, int c)
{
   return (next = next * a + c);
}

// initialize A, C and generate the first random number in parallel
int prand_init(int np, int pid, int *A, int *C, int a, int c)
{
   int i;

   *A = 1; *C = 0;
   for (i=0; i<np; i++) {
      *A = (a * *A);
      *C = (a * *C + c);
   }

   for (i=0; i<pid; i++)
      rand2(a, c);

   return rand2(a, c);
}

void sort(int n, int arr[n]) {
   int i, j, tmp;
   for (i=0; i < n-1; i++) {
      for (j=0; j < n-1; j++) {
         if (arr[j] > arr[j+1]) {
            tmp = arr[j];
            arr[j] = arr[j+1];
            arr[j+1] = tmp;
         }
      }
   }
}

void merge(int n, int arr[n]){
   int sorted[n];
   int i, j, k, l, half;

   i = 0;
   half = n/2;
   j = half;
   k = 0;

   while ((i < half) && (j < n)){
      if(arr[i] <= arr[j])
         sorted[k++] = arr[i++];
      else
         sorted[k++] = arr[j++];
   }

   if (i == half){
      for (l=j; l<n; l++)
         sorted[k++] = arr[l];
   }
   else {
      for(l=i; l<half; l++)
        sorted[k++] = arr[l];
   } 

   for(l=0; l < n; l++){
      arr[l] = sorted[l];
   }
}

// check if a square number
int chk_square(int np)
{
   int i, n = 0;

   for (i=0; i<sizeof(int)*8; i++)
      if (np&1<<i) n++;
   return n; // not a square number if n is not 1
}
