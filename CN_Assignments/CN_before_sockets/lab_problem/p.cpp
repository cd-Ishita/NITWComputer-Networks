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
static int pp=0;
static struct sigaction siga;
key_t key=ftok(".",65);

struct shmem
{
	int i;
};
struct shmem *z;
int k;


int f=1;



static void multi_handler(int sig, siginfo_t *siginfo, void *context) 
{
    // get pid of sender,

  //  if(sig==SIGUSR2)
  //  {
  //  	f=0;
   // }
 //   else
   // {

    	pid_t sender_pid = siginfo->si_pid;
        cout<<"signal from    "<<sender_pid<<endl;
        pp++;
        sleep(1);
   //     kill((pid_t)k,SIGUSR2);
  // }
}

int main()
{
	cout<<"process of p22:"<<getpid()<<endl;
	siga.sa_sigaction = *multi_handler;
    	siga.sa_flags |= SA_SIGINFO;
    	sigaction(SIGUSR1, &siga, NULL);
    	int shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
    	z = (struct shmem*)shmat(shmid,NULL,0);
    	z->i=getpid();
	int fd;
	mkfifo("Hero1",0666);
	mkfifo("Hero2",0666);
	
	char ch1[1024],ch2[1024];
	while(1)
	{
		cout<<pp<<endl;
		
		if(pp%2==1)
		{
		/*
			cout<<"hello"<<endl;
			int A[2];
			int B[2];
			
			pipe(A);
			int c=fork();
			if(c>0)
			{	
				fd = open("Hero", O_RDONLY); 
        			read(fd, ch2, 1024); 
   		    // 	printf("User1: \n"); 
    			    	close(fd); 
				close(A[0]);
				close(B[1]);
				
					char *buffer1=ch2;
					write(A[1],buffer1,1024);
						
				
				close(A[1]);
				close(B[0]);
		
		
			}
			else
			{
				char buffer1[1024];
				close(A[1]);
				close(B[0]);
				while(1)
				{
			
					read(A[0],buffer1,1024);
					
					int i;
					if(buffer1[0])
					cout<<buffer1<<endl;
					memset(buffer1,0,sizeof buffer1);
		
				}
				close(A[0]);
				close(B[1]);
			
			}
		*/
		
		
		
			int A[2];
			int B[2];
			
			pipe(A);
			int c=fork();
			if(c>0)
			{	
				fd = open("Hero", O_RDONLY); 
        			read(fd, ch2, 1024); 
   		    // 	printf("User1: \n"); 
    			    	close(fd); 
				close(A[0]);
				close(B[1]);
				
					char *buffer1=ch2;
					write(A[1],buffer1,1024);
						
				
				close(A[1]);
				close(B[0]);
		
		
			}
			else
			{
				char buffer1[1024];
				close(A[1]);
				close(B[0]);
			
			
				setpgid(getpid(),0);
				int G1=getpgrp();
				while(1)
				{
				
					if(read(A[0],buffer1,1024)>0)
					{
						cout<<"s1:"<<buffer1<<endl;
					}
				
				}
				
				
				close(A[0]);
				close(B[1]);
			
			}
		
		
		
		}
		else
		{
			fd = open("Hero", O_RDONLY); 
        		read(fd, ch2, 1024); 
   		    //	printf("User1: \n"); 
    		    	close(fd); 
		
			FILE* g2fd1=popen("./popen1","w");
			FILE* g2fd2=popen("./popen2","w");
			char *buffer=ch2;
			write(fileno(g2fd1),buffer,1024);
			write(fileno(g2fd2),buffer,1024);
			
		
		
		
		
		
		
		
		
		
		
		}
		
	}

	
    	
    	
/*    	
   	cout<<"p2="<<z->i<<endl;
   	while(f);
    	k=z->i;

    	kill((pid_t)k,SIGUSR2);	*/
    	while(1);

    return 0;

}
