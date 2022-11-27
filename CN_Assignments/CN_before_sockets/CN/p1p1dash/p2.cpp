#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <semaphore.h>
#include <fcntl.h>
#include<pthread.h>
#include<cstring>
#include<iostream>
using namespace std;
char buffer[1024];
const char* name1="pp";
const char* name2="pc";
int main(){
    
    sem_t* pp=sem_open(name1,O_CREAT|O_EXCL,0666,0);
    sem_t* pc=sem_open(name2,O_CREAT|O_EXCL,0666,1);
    
    int fd=open("simple.txt",O_RDONLY);
   
    int c=fork();
    
    int n=0;
    if (c>0)
	{
        while((n=read(fd,buffer,10))>0)
		{ 
			sem_t *pp = sem_open(name1,O_EXCL);
			sem_t *pc = sem_open(name2,O_EXCL);
			sem_wait(pc);
			cout<<"parent:\t"<<buffer<<endl;
            
            sem_post(pp);
            sleep(1);
        }
       
    }
    else
	{
        while((n=read(fd,buffer,10))>0)
		{ 
			sem_t *pp = sem_open(name1,O_EXCL);
			sem_t *pc = sem_open(name2,O_EXCL);
            sem_wait(pp);
            cout<<"child:\t "<<buffer<<endl;
            sem_post(pc);
            sleep(1);
        }
        
    }
    sem_unlink(name1);
    sem_unlink(name2);
    close(fd);
}
