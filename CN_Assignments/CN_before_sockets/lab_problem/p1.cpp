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
int k,shmid;
struct shmem *z;

/*

static void multi_handler(int sig, siginfo_t *siginfo, void *context) 
{
    // get pid of sender,
    pid_t sender_pid = siginfo->si_pid;
    cout<<"signal from"<<sender_pid<<endl;
    sleep(1);
   kill((pid_t)k,SIGUSR1);
}
*/

int main()
{

	cout<<"process of p1:"<<getpid()<<endl;
//	siga.sa_sigaction = *multi_handler;
  //  	siga.sa_flags |= SA_SIGINFO;
  //  	sigaction(SIGUSR2, &siga, NULL);
    	shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
	z = (struct shmem*)shmat(shmid,NULL,0);
	
    	k=z->i;
   // 	z->i=getpid();
	while(1)
	{
		cout<<"hi"<<endl;
		kill((pid_t)k,SIGUSR1);
		sleep(10);
	
	}
    	
    	 
    	
    	
  //	cout<<"p1="<<z->i<<endl;
  	while(1);

}
