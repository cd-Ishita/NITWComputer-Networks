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
#include <semaphore.h>
using namespace std;

const char* sem_1="s3"; 
const char* sem_2="f";

struct mymsg
{
	long mtype;
	char mdata[2048];	
};

int main()
{
	sem_t *s3 = sem_open(sem_1,0);
	sem_t *s = sem_open(sem_2,0);
	
	map<long,string >m;
	m[getpid()]="p3";
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	struct mymsg *msg;
	msg=new(struct mymsg);
	msg->mtype=getpid();
	strcpy(msg->mdata,"p3");
	msgsnd(msqid,msg,sizeof(msg), 0);
	msgsnd(msqid,msg,sizeof(msg), 0);
	int count =0;
	sleep(10);
	while(count<2)
	{
		cout<<count<<endl;
		struct mymsg *msg1;
		msg1=new(struct mymsg);
		if(msgrcv(msqid, msg1, sizeof(msg1),0,0)!=-1)
		{
		
		if(m.find(msg1->mtype)!=m.end())
		{
			msgsnd(msqid,msg1,sizeof(msg1),0);
			
		}
		else
		{
			m[msg1->mtype]=msg->mdata;
			count++;
			
		
		}
		
		}
	
	}
	cout<<"hello3"<<endl;
	
	sem_post(s);
	sem_wait(s3);
	
	cout<<"welcome p3"<<endl;
	map<long,string >::iterator i;
	for(i=m.begin();i!=m.end();i++)
	{
		cout<<i->first<<endl;
	}
	
	//chat
	
	
	
	
	
	
	msgctl(msqid, IPC_RMID, NULL);

}
