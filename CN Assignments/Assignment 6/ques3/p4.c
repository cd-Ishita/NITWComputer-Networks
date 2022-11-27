#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>
#include<semaphore.h>
int main(){
    sem_t *sem34 = sem_open("S34", O_CREAT, 0666, 0);
    sem_t *sem41 = sem_open("S41", O_CREAT, 0666, 0);

    printf("I am P4. I am waiting for semaphore S34\n");
    sem_wait(sem34);
    printf("I got semaphore S34 signalling from P3\n");
    printf("Enter any character to sem-signal (S41)\n");
    int c;
    scanf("%d", &c);
    printf("I am signalling semaphore signal of S41\n");

    sem_post(sem41);
}