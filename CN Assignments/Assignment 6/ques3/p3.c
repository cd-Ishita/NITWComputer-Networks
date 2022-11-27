#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>
#include<semaphore.h>
int main(){
    sem_t *sem23 = sem_open("S23", O_CREAT, 0666, 0);
    sem_t *sem34 = sem_open("S34", O_CREAT, 0666, 0);

    printf("I am P3. I am waiting for semaphore S23\n");
    sem_wait(sem23);
    printf("I got semaphore S23 signalling from P2\n");
    printf("Enter any character to sem-signal (S34)\n");
    int c;
    scanf("%d", &c);
    printf("I am signalling semaphore signal of S34\n");

    sem_post(sem34);
}