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

const char* sem_1="s2"; 
const char* sem_2="f";

struct mymsg
{
	long mtype;
	char mdata[2048];	
};

int main()
{
//	cout<<"p2 pid in p2"<<getpid()<<endl;
	sem_t *s2 = sem_open(sem_1,0);
	sem_t *s = sem_open(sem_2,0);
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	
	sem_wait(s2);
	
	struct mymsg *msg1;
	msg1=new(struct mymsg);
	msgrcv(msqid, msg1, sizeof(msg1),0,0);
	
	cout<<"recieved PID in C2"<<endl;
	cout<<"message: "<<msg1->mdata<<endl;
		
	struct mymsg *msg;
	msg=new(struct mymsg);
	msg->mtype=getpid();
	strcpy(msg->mdata,"p2");
	msgsnd(msqid,msg,sizeof(msg), 0);
	
	
	
	cout<<"hello2"<<endl;
	
	sem_post(s);
	sem_wait(s2);
	cout<<"welcome p2"<<endl;
	
	
	//chat
	
	
	msgctl(msqid, IPC_RMID, NULL);

}
