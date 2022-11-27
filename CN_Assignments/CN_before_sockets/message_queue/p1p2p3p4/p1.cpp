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
	map<long,string >m;
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	struct mymsg *msg;
	msg=new(struct mymsg);
	while(msgrcv(msqid, msg, sizeof(msg),0,0)!=-1)
	{
		
		if(m.find(msg->mtype)!=m.end())
		cout<<m[msg->mtype]<<" : "<<msg->mdata<<endl;
		else
		{
			m[msg->mtype]=msg->mdata;
		
		}
		
	}
	
	
	msgctl(msqid, IPC_RMID, NULL);

}
