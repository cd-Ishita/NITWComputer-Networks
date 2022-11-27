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
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
static int cnt=0;
struct pollfd pfds[2];
struct mymsg
{
	long mtype;
	char mdata[2048];	
};
int nsfd;
int sfd1,sfd2;
key_t key=ftok("message_queue",65);
int msqid=msgget(key,0666|IPC_CREAT);

void* f1(void* arg)
{	
	
	while(1)
	{
	//	cout<<"hi"<<endl;
		struct mymsg *msg;
		msg=new(struct mymsg);
		if(msgrcv(msqid, msg, sizeof(msg),0,0)!=-1)
		{
			cout<<msg->mtype<<" : "<<msg->mdata<<endl;
		}	
	}
}

void* f2(void* arg)
{
	while(1)
	{
		int count = poll(pfds,2,5000);
		if(count==-1)
		perror("poll error");
		else if(count==0)
		{printf("time out\n");
		break;}
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<2;i++)
			{
				char ch[120];
				if(pfds[i].revents && POLLIN  )
				{	
   				 if ((nsfd = accept(pfds[i].fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    				 { 
        			 perror("accept"); 
        			 exit(EXIT_FAILURE); 
    				 } 
    				 int c=fork();
    				 if(c==0)
    				 {
				    close(pfds[i].fd);
				    dup2(nsfd,0);
    				    if(i==0)
 				    execv("./s1",NULL);
				    else if(i==1)
    				    {
				    execv("./s2",NULL);
    				    }
    				}
				pfds[i].revents=0;
				}
				else
				free++;
			}
			if(free == 2)
			break;
		}
		pfds[0].fd=sfd1;
		pfds[0].events=POLLIN;
		pfds[0].revents=0;
		pfds[1].fd=sfd2;
		pfds[1].events=POLLIN;
		pfds[1].revents=0;
	}





} 


int main(int argc, const char* argv[]) 
{ 

	sleep(10);
    
    int valread;
    int port[2];
    port[0]=8080;
    port[1]=8081;
    
    char buffer[1024] = {0};    
    
    
    	if ((sfd1 = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
       
    	if (setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   	address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( port[0] ); 
       
    	if (bind(sfd1, (struct sockaddr *)&address,sizeof(address))<0) 
    	{ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (listen(sfd1, 3) < 0) 
    	{ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    	}
   
   
   	if ((sfd2 = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
       
    	if (setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   	address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( port[1] ); 
       
    	if (bind(sfd2, (struct sockaddr *)&address,sizeof(address))<0) 
    	{ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (listen(sfd2, 3) < 0) 
    	{ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    	}
   	
	pfds[0].fd=sfd1;
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	pfds[1].fd=sfd2;
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	pthread_t t1,t2;
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);
	pthread_join(t2,NULL);
	pthread_join(t1,NULL);
	
	while(1);
	close(sfd1);
	close(sfd2);
	return 0;
}

