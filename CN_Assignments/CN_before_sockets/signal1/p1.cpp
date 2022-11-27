#include <time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/ether.h>
#include <netinet/udp.h>
#include <semaphore.h>
#include <bits/stdc++.h>
using namespace std;
int *X, *Y;
key_t key1, key2;
int shmid1, shmid2;
static void handler1(int signo)
{
	cout << "PROCESS 1 RECEIVED SIGNAL\n";
	int ppid = *Y;
	
	cout << "Process 1 Read: " << ppid << "\n";
	*X = ppid;
    	cout << "WRITTEN BY PROCESS 1\n";
    	sleep(1);
    	kill(ppid, SIGUSR1);
}


int main()
{
	signal(SIGUSR2, handler1);
	key_t key1=ftok("/x",65);
	shmid1 = shmget(key1,1024,0666|IPC_CREAT);
	X = (int *) shmat(shmid1,(void *)0,0);
	*X = 0;
	shmid2 = shmget(key2,1024,0666|IPC_CREAT);
	Y = (int *) shmat(shmid2,(void *)0,0);
	*Y = 0;
	pid_t c = fork();
    	if (c>0) //child program
    {
    	int parentPid = getpid();

        cout << "PID of process P1 = " << parentPid << endl;
        *X = parentPid;
        while (1)
        {
            cout << "process 1 is waiting for signal\n";
            sleep(1);
        }

        
    }
    else
    {
        cout << "IN CHILD PROCESS, ABOUT TO EXECUTE P2\n";
        execl("./p2", NULL);
    }

}
