#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include <sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<poll.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
using namespace std;

static struct sigaction siga;
key_t key=ftok(".",65);

struct shmem
{
	int i;
};
struct shmem *z;
int k;


int f=1;
int cnt=0;
static void multi_handler(int sig, siginfo_t *siginfo, void *context) 
{
    	pid_t sender_pid = siginfo->si_pid;
    	cnt=1;
        cout<<"signal from    "<<sender_pid<<endl;
        sleep(1);
}

int main()
{

	siga.sa_sigaction = *multi_handler;
    	siga.sa_flags |= SA_SIGINFO;
    	sigaction(SIGUSR1, &siga, NULL);
    	
    	int shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
    	z = (struct shmem*)shmat(shmid,NULL,0);
    	z->i=getpid();

	
	int fd2=fileno(popen("./p2","r"));
	int fd3=fileno(popen("./p3","r"));
	int fd4=fileno(popen("./p4","r"));
	
	
	
	int fd5=fileno(popen("./p5","w"));
	int fd6=fileno(popen("./p6","w"));

	struct pollfd pfds[3];
	
//	
//	dup2(fd6,1);
	
	
	while(1)
	{
		pfds[0].fd=fd2;
		pfds[0].events=POLLIN;
		pfds[1].fd=fd3;
		pfds[1].events=POLLIN;
		pfds[2].fd=fd4;
		pfds[2].events=POLLIN;
		
		
		
		int count = poll(pfds,3,0);
		if(count>0)
		{
		for(int i=0;i<3;i++)
		{
			if(pfds[i].revents && POLLIN)
			{	
			
				if(cnt==0)
				{
					dup2(0,20);
				dup2(pfds[i].fd,0);
				string s;
				getline(cin,s);
				dup2(1,66);
				dup2(fd5,1);
				cout<<"p5"<<s<<endl;
				dup2(66,1);
				pfds[i].revents=0;
				dup2(20,0);
				
				
				
				}
				else
				{
				
					dup2(0,20);
				dup2(pfds[i].fd,0);
				string s;
				getline(cin,s);
				dup2(1,67);
				dup2(fd6,1);
				cout<<"p6"<<s<<endl;
				dup2(67,1);
				pfds[i].revents=0;
				dup2(20,0);
				
				
				
				
				
				}
				
					
			}
		
		
		
		
		}
			
		
		
		}

	}
	
	return 0;
}
