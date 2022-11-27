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
		{
			long l;
			map<long,string >:: iterator it;
			string s="";
			s+=msg->mdata[0];
			for(it=m.begin();it!=m.end();it++)
			{
				if(it->second==s)
				{
					l=it->first;
					break;
				}
			}
			msg->mdata[0]=m[l][0];
			cout<<m[msg->mtype]<<" : "<<msg->mdata<<endl;
			
			string str=msg->mdata;
			struct mymsg * msg1;
			msg1=new struct mymsg;
			msg1->mtype=l;
			strcpy(msg1->mdata,str.c_str());
			msgsnd(msqid,msg1,sizeof(msg1), 0);
		
		}
		
		else
		{
			m[msg->mtype]=msg->mdata;
		
		}
		
	}
	
	
	msgctl(msqid, IPC_RMID, NULL);

}
