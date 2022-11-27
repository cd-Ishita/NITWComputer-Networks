//compile with -lpthread -lrt
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>
#include<semaphore.h>
int main(){
    sem_t *sem12 = sem_open("S12", O_CREAT, 0666, 0);
    sem_t *sem41 = sem_open("S41", O_CREAT, 0666, 0);

    printf("I am P1. Ener any character to sem-signal(S12)\n");
    int c;
    scanf("%d", &c);
    printf("I am signalling semaphore signal of S12\n");

    sem_post(sem12);

    printf("I am waiting for semaphore S41\n");
    sem_wait(sem41);

    printf("I got semaphore signalling from P4\n");
}