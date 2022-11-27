#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<bits/stdc++.h>
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
key_t key=ftok("message_queue",65);
int msqid=msgget(key,0666|IPC_CREAT);
int main()
{

	
	int nsfd=dup(0);
	while(1)
	{
		char buffer[1024]={'\0'};
		recv(nsfd,buffer,1024,0);
		
		cout<<buffer<<endl;
		struct mymsg * msg;
		msg=new struct mymsg;
		msg->mtype=getpid();
		strcpy(msg->mdata,buffer);
		msgsnd(msqid,msg,sizeof(msg->mdata), 0);
	
	}

}
