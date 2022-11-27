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
struct pollfd pfds[10];
int nsfd;
int sfd[10];
struct add{
	int port;
	string path;

};
int i=0;
vector<struct add > V;

void* f1(void* arg)
{	
	
	while(1)
	{
		struct add a;
		cin>>a.port>>a.path;
		V.push_back(a);
		
	if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
       
    	if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   	address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( a.port ); 
       
    	if (bind(sfd[i], (struct sockaddr *)&address,sizeof(address))<0) 
    	{ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (listen(sfd[i], 3) < 0) 
    	{ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    	}
    	
    	pfds[i].fd=sfd[i];
	pfds[i].events=POLLIN;
	pfds[i].revents=0;
    	
		
	}
}

void* f2(void* arg)
{
	while(1)
	{
		int count = poll(pfds,i,5000);
		if(count==-1)
		perror("poll error");
		else if(count==0)
		{printf("time out\n");
		break;}
		else
		{
			int c=0;
			int free=0;
			for(int ii=0;ii<i;ii++)
			{
				char ch[120];
				if(pfds[ii].revents && POLLIN  )
				{	
   					 if ((nsfd = accept(pfds[ii].fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    					 { 
        					 perror("accept"); 
        					 exit(EXIT_FAILURE); 
    					 } 
    					 int c=fork();
    					 if(c==0)
    					 {
					    close(pfds[ii].fd);
					    dup2(nsfd,0);
					    const char* cc=V[ii].path.c_str();
 					    execv(cc,NULL);
				    
    					 }
    					 else
    					 {
    					 	exit(0);
    					 }
					 pfds[ii].revents=0;
				}
				else
				free++;
			}
			if(free == i)
			break;
		}
	}





} 


int main(int argc, const char* argv[]) 
{ 
	char buffer[1024] = {0};    
	pthread_t t1,t2;
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
	while(1);
	return 0;
}

