#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct {
   char s;
   int data;
} SHM;

main(int argc, char *argv[])
{
   int shm_id;
   void *shm_addr;
   SHM *shm;
   int key;
   int sum = 0;

   if(argc !=2) {
      printf("usage: %s key\n", argv[0]);
      exit(1);
   }
   
   key = atoi(argv[1]);

   shm_id = shmget(key, sizeof(int), IPC_CREAT|0666);
   shm_addr = (void *)shmat(shm_id, 0, 0);
   shm = (SHM *)shm_addr;

   printf("sum: %d\n", sum);

   do {
      if(shm->s == 1) {
	 sum += shm->data;
	 printf("sum: %d\n", sum);
	 shm->s = 0;
      }
   } while(shm->s != -1);

   shmdt((void *)shm_addr);
   shmctl(shm_id, IPC_RMID, 0);

   exit(0);
}
