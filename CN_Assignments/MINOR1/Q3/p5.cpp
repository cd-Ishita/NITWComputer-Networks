#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>      
#include <sys/wait.h>
#include<bits/stdc++.h>
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
#define e 1024

static struct sigaction siga;
key_t key=ftok(".",65);

struct shmem
{
	int i;
};
int k,shmid;
struct shmem *z;

int main()
{
	shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
	z = (struct shmem*)shmat(shmid,NULL,0);
	
    k=z->i;
    string str;
    getline(cin, str);
    cout<<str<<endl;
    kill((pid_t)k,SIGUSR1);
    exit(0);
    
}
