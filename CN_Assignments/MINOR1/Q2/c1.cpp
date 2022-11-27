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

const char* sem_1="s1"; 
const char* sem_2="f";

struct mymsg
{
	long mtype;
	char mdata[2048];	
};

int main()
{
	sem_t *s1 = sem_open(sem_1,0);
	sem_t *s = sem_open(sem_2,0);
	
	
	key_t key;
	key=ftok("message_queue",65);
	int msqid=msgget(key,0666|IPC_CREAT);
	
//	cout<<"p1 pid in p1"<<getpid();
	
	
	
	
	struct mymsg *msg;
	msg=new(struct mymsg);
	msg->mtype=getpid();
	strcpy(msg->mdata,"p1");
	msgsnd(msqid,msg,sizeof(msg), 0);
	
	sem_post(s);
	cout<<"hi1"<<endl;
	sem_wait(s1);
	cout<<"hello1"<<endl;
	
	
	
	struct mymsg *msg1;
	msg1=new(struct mymsg);
	msgrcv(msqid, msg1, sizeof(msg1),0,0);
	
	cout<<"recieved PID in C1"<<endl;
	cout<<"message "<<msg1->mdata<<endl;
	
	

	
	sem_post(s);
	sem_wait(s1);
	
	cout<<"welcome p1"<<endl;
	
	
	
	//chat
	
	
	msgctl(msqid, IPC_RMID, NULL);

}
