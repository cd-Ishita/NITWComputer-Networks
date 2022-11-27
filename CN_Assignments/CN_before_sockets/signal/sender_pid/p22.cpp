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
/*    	
   	cout<<"p2="<<z->i<<endl;
   	while(f);
    	k=z->i;

    	kill((pid_t)k,SIGUSR2);	*/
    	while(1);

    return 0;

}
