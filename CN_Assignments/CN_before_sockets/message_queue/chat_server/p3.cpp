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
int msqid;
long pid=getpid();
struct mymsg
{
	long mtype;
	char mdata[2048];	
};

static void* readthread(void* arg)
{
	cout<<"hi"<<endl;
	while(1){
	
		struct mymsg * msg;
		msg=new struct mymsg;
		msgrcv(msqid, msg, sizeof(msg->mdata),pid,0);
		
			if(!sizeof(msg->mdata))
			cout<<msg->mdata<<endl;
		
		
		
		}
		cout<<"hellllll"<<endl;
}
static void* writethread(void* arg)
{
	string str;
	cout<<"enter message:";
	while(getline(cin,str))
	{	if(str[0])
		{
		struct mymsg * msg;
		msg=new struct mymsg;
		msg->mtype=pid;
		strcpy(msg->mdata,str.c_str());
		msgsnd(msqid,msg,sizeof(msg->mdata), 0);
		cout<<"enter message:";
		}
		
		
	}
}

int main()
{
	pthread_t t1,t2;
	struct mymsg message[10];
	key_t key;
	key=ftok("message_queue",65);
	msqid=msgget(key,0666|IPC_CREAT);
	struct mymsg * msg;
	msg=new struct mymsg;
	msg->mtype=getpid();
	strcpy(msg->mdata,"3");
	msgsnd(msqid,msg,sizeof(msg->mdata), 0);
	
	pthread_create(&t1,NULL,readthread,NULL);
	pthread_create(&t2,NULL,writethread,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
	
	
	msgctl(msqid, IPC_RMID, NULL);

}

