#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
   int data;

   if(argc != 3) {
      fprintf(stderr, "usage: %s key data\n", argv[0]);
      exit(1);
   }
   
   key = atoi(argv[1]);
   data = atoi(argv[2]);

   shm_id = shmget(key, 0, 0);
   shm_addr = (void *)shmat(shm_id, 0, 0);
   shm = (SHM *)shm_addr;

   if(data == 9999) {
      shm->s = -1;
   } else {
      shm->s = 1;
   }

   shm->data = data;

   shmdt((void *)shm_addr);

   exit(0);
}
