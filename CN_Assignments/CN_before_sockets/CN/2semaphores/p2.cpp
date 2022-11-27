#include <stdio.h>               
#include <unistd.h>
#include <sys/types.h>      
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <errno.h>          
#include <semaphore.h>      
#include <fcntl.h>          
#include<pthread.h>
#include<cstring>
#include<iostream>
using namespace std;
const char* sem_1="1"; 
const char* sem_2="2";
int main()
{
	sem_t *s = sem_open(sem_1,0);
	sem_t *t = sem_open(sem_2,0);
	if(s == SEM_FAILED || t == SEM_FAILED)
	{
		cout<<"Error Occured"<<endl;    
        	sem_unlink(sem_1);
        	sem_unlink(sem_2);
        	exit(1);
	}
	key_t key1 = ftok("a",65); 
    key_t key2 = ftok("b",66);
	int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
    int shmid2= shmget(key2,1024,0666|IPC_CREAT);
    char* sharedmemory1 =(char *)shmat(shmid1,(void *)0,0); 
	char* sharedmemory2 =(char *)shmat(shmid2,(void *)0,0); 
	int c=fork();
	int x=0,y=0;
	char str[4096];
	while(x<=50 && y<=50)
	{
		sem_wait(t);
		x=stoi(sharedmemory1);
		y=x+1;
		if(y>50)
		{
			sem_unlink(sem_1);
    			sem_unlink(sem_2);
    			shmdt(sharedmemory1);
    			shmctl(shmid1,IPC_RMID,NULL); 
    			shmdt(sharedmemory2);
    			shmctl(shmid2,IPC_RMID,NULL); 
			break;
		}
		cout<<"P2 updated the value of y which is "<<y<<endl;
		sprintf(str,"%d",y);
		strcpy(sharedmemory2,str);
		sem_post(s);
        sleep(1);
	}
}
