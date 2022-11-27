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
#include<bits/stdc++.h>
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
	struct mymsg * msg;
	msg=new struct mymsg;
	msg->mtype=getpid();
	strcpy(msg->mdata,"p2");
	msgsnd(msqid,msg,sizeof(msg), 0);
	
	string str;
	cout<<"enter message:";
	while(getline(cin,str))
	{
		struct mymsg * msg;
		msg=new struct mymsg;
		msg->mtype=getpid();
		strcpy(msg->mdata,str.c_str());
		msgsnd(msqid,msg,sizeof(msg), 0);
		cout<<"enter message:";
		
	}
	
//	msgctl(msgid, IPC_RMID, NULL);

}

