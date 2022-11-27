#include <sys/shm.h>
//compile with -lpthread -lrt
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>
#include<semaphore.h>
#include <math.h>
#include<string.h>

int main(){
    sem_t *S1 = sem_open("S1", O_CREAT, 0666, 0);
    sem_t *S2 = sem_open("S2", O_CREAT, 0666, 0);

    // ftok to generate unique key
    key_t key1 = ftok("x",65);
  
    // shmget returns an identifier in shmid
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    char *data1 = (char*) shmat(shmid1,(void*)0,0);

    // ftok to generate unique key
    key_t key2 = ftok("y",65);
  
    // shmget returns an identifier in shmid
    int shmid2 = shmget(key2,1024,0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    char *data2 = (char*) shmat(shmid2,(void*)0,0);

    
    while(1){
        printf("I am waiting for S1\n");
        sem_wait(S1);

        int temp = atoi(data1);
        printf("This is temp %d\n", temp);

        temp++;
        sprintf(data1,"%d", temp);

        printf("Enter any char to signal S2\n");
        int c = getchar();
        sem_post(S2);
    }
}