#include<bits/stdc++.h>
#include <pthread.h>
#include <sys/wait.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h> 
using namespace std;
const char* sem_name = "state";
void p1()
{
	sem_t *s = sem_open(sem_name,O_EXCL);

	sem_wait(s);
	cout<<"s1 : im p1,im printed\n";
	cout<<"s2 : im waiting for p2 \n";
	sem_post(s);
	cout<<"s3 : i was signaled by p2 \n"; 
	
}
void p2()
{
	sem_t *s = sem_open(sem_name,O_EXCL);
	sem_wait(s);
	cout<<"s4 : im p2,im printed\n";
	cout<<"s5 : im waiting for p1 \n";
	sem_post(s);
	cout<<"s6 : i was signaled by p1 \n"; 
}

int main()
{
	int c=0;
	c=fork();
	sem_t *s = sem_open(sem_name,O_CREAT|O_EXCL,0666,1);
	int i=5;
	while(i--)
	{
		if(c>0)
		{
			p1();
		}
		else
		{
			p2();
		}
	}
	sem_unlink(sem_name);
}


