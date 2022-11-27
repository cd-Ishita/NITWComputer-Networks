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

const char* sem_1="s4"; 
const char* sem_2="f";

struct mymsg
{
	long mtype;
	char mdata[2048];	
};

int main()
{
//	cout<<"p4 pid in p4"<<getpid();
	sem_t *s4 = sem_open(sem_1,0);
	sem_t *s = sem_open(sem_2,0);
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	
	
	sem_wait(s4);
	
	struct mymsg *msg1;
	msg1=new(struct mymsg);
	msgrcv(msqid, msg1, sizeof(msg1),0,0);
	
	cout<<"recieved PID in C4"<<endl;
	cout<<"message: "<<msg1->mdata<<endl;
		
	struct mymsg *msg;
	msg=new(struct mymsg);
	msg->mtype=getpid();
	strcpy(msg->mdata,"p4");
	msgsnd(msqid,msg,sizeof(msg), 0);
	
	
	
	cout<<"hello4"<<endl;
	
	sem_post(s);
	sem_wait(s4);
	cout<<"welcome p4"<<endl;
	
	
	//chat
	
	
	msgctl(msqid, IPC_RMID, NULL);

}
