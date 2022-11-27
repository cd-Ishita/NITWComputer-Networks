#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
using namespace std;
const char* name1="pp";
const char* name2="pc";
char buffer[1024];
int main()
{
	
	sem_t* pp=sem_open(name1,O_CREAT|O_EXCL,0666,0);
    sem_t* pc=sem_open(name2,O_CREAT|O_EXCL,0666,1);
	int fd=open("simple.txt",O_RDONLY),cnt=0;
	dup2(fd,0);
	int c=fork();
	if(c>0)
	{
		while((cnt=read(fd,buffer,10))>0)
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
		execv("./p2",nullptr);
	}
}
