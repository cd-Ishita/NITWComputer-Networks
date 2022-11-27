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
	long mtype;
	char mdata[2048];	
};

int main()
{
	struct mymsg message[10];
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	
	message[0].mtype=1;
	message[1].mtype=2;
	message[2].mtype=3;
	message[3].mtype=1;
	message[4].mtype=2;
	strcpy(message[0].mdata,"1_1_helloworld");
	strcpy(message[1].mdata,"2_2_helloworld");
	strcpy(message[2].mdata,"3_3_helloworld");
	strcpy(message[3].mdata,"4_1_helloworld");
	strcpy(message[4].mdata,"5_2_helloworld");
	
	
	for(int i=0;i<5;i++)
	{
		struct mymsg * msg;
		msg=new struct mymsg;
		msg->mtype=message[i].mtype;
		strcpy(msg->mdata,message[i].mdata);
		msgsnd(msqid,msg,sizeof(msg->mdata), 0);
	
	}
	
//	msgctl(msgid, IPC_RMID, NULL);

}

