#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
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
struct mymsg
{
	long mytype;
	char mdata[2048];	
};

int main()
{
	
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	for(int i=0;i<2;i++)
	{
		struct mymsg *msg;
		msg=new(struct mymsg);
		msgrcv(msqid, msg, sizeof(msg), 1, 0); 
		cout<<msg->mdata<<endl;
	
	
	
	}
	msgctl(msqid, IPC_STAT, NULL);
	msgctl(msqid, IPC_RMID, NULL);

}
