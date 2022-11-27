#include<stdio.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <fcntl.h>
#include<iostream>
using namespace std;
const char* sem_name1 = "state1";
const char* sem_name2 = "state2";
char buffer[10];
int n;
void p2(int fd)
{
    sem_t *mutex1 = sem_open(sem_name1,O_EXCL);
    sem_t *mutex2 = sem_open(sem_name2,O_EXCL);
	sem_wait(mutex2);
	while((n=read(fd,buffer,10))>0)
	{
    	cout<<"child:\t"<<buffer<<endl;
    	sem_post(mutex1);
    	sem_wait(mutex2);
 	} 
}
void p1(int fd)
{
    sem_t *mutex1 = sem_open(sem_name1,O_EXCL);
    sem_t *mutex2 = sem_open(sem_name2,O_EXCL);
	while((n=read(fd,buffer,10))>0)
	{
    	cout<<"parent:\t"<<buffer<<endl;
    	sem_post(mutex2); 
    	sem_wait(mutex1);
	}
	sem_post(mutex2);  
}
int main() 
{ 
    sem_t *mutex1 = sem_open(sem_name1,O_CREAT|O_EXCL,0666,0);
    sem_t *mutex2 = sem_open(sem_name2,O_CREAT|O_EXCL,0666,0);
    int c=fork();
    int fd=open("simple.txt",O_RDONLY);
    if (c>0)
	{
		p1(fd);
	}	
	else
	{
		p2(fd);
	}
    sem_unlink(sem_name1);
    sem_unlink(sem_name2); 
    close(fd);
    return 0; 
}
