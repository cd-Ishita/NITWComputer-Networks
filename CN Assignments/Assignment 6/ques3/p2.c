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
    sem_t *sem23 = sem_open("S23", O_CREAT, 0666, 0);

    printf("I am P2. I am waiting for semaphore S12\n");
    sem_wait(sem12);
    printf("I got semaphore S12 signalling from P1\n");
    printf("Enter any character to sem-signal (S23)\n");
    int c;
    scanf("%d", &c);
    printf("I am signalling semaphore signal of S23\n");

    sem_post(sem23);
}