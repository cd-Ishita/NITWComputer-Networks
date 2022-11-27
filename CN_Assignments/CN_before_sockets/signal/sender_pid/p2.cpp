
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

static int int_count = 0, max_int = 5;
static struct sigaction siga;


key_t key=ftok(".",65);
int f=1;
struct shmem
{
	int i;
};
struct shmem *z;
int shmid,other;

static void multi_handler(int sig, siginfo_t *siginfo, void *context) 
{
    // get pid of sender,

    if(sig==SIGUSR2)
    {
    	f=0;
    }
    else
    {

    	pid_t sender_pid = siginfo->si_pid;
        cout<<"signal from    "<<sender_pid<<endl;
        sleep(1);
        kill((pid_t)other,SIGUSR2);
   }
}

int main() 
{
    // print pid
    printf("process [%d] started.\n", (int)getpid());

    // prepare sigaction
    siga.sa_sigaction = *multi_handler;
    siga.sa_flags |= SA_SIGINFO; // get detail info
    sigaction(SIGUSR1, &siga, NULL);
    sigaction(SIGUSR2, &siga, NULL);


    shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
    z = (struct shmem*)shmat(shmid,NULL,0);
    z->i=getpid();
    cout<<"p2="<<z->i<<endl;
    while(f);
    other=z->i;
    cout<<"p1 in p2="<<other<<endl;

    kill((pid_t)other,SIGUSR2);
    while(1);

    return 0;
}
