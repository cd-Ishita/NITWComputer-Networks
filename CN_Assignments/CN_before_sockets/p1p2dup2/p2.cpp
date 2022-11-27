#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
using namespace std;
char buffer[1024];
const char* name1="pp";
const char* name2="pc";
int main()
{
	char buffer[10];
	int cnt=0;
	while((cnt=read(0,buffer,10))>0)
	{
			sem_t *pp = sem_open(name1,O_EXCL);
			sem_t *pc = sem_open(name2,O_EXCL);
            sem_wait(pp);
            cout<<"child:\t "<<buffer<<endl;
            sem_post(pc);
            sleep(1);
	}
	
}
