#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include<bits/stdc++.h>
using namespace std;
int cnt=0;
int A[3]={0};
int sfd[3];
pthread_t tid,tid0,tid1,tid2;
struct pollfd pfds[3];
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
void* func1(void * arg)
{
	while(1)
	{
		if(A[0])
		{
			cout<<"train reached platform1";
    			std::this_thread::sleep_for(10000ms);
    			cnt--;
    			cout<<"\ncount:"<<cnt<<endl;
    			A[0]=0;
		
		
		}
		
	
	
	}
    	
    	pthread_exit((void*)0);
}
void* func2(void * arg)
{
	while(1)
	{
		if(A[1])
		{
			cout<<"train reached platform2";
    			std::this_thread::sleep_for(10000ms);
    			cnt--;
    			cout<<"\ncount:"<<cnt<<endl;
    			A[1]=0;
		
		
		}
		
	
	
	}
    	pthread_exit((void*)0);
}
void* func3(void * arg)
{
	
    	while(1)
	{
		if(A[2])
		{
			cout<<"train reached platform3";
    			std::this_thread::sleep_for(10000ms);
    			cnt--;
    			cout<<"\ncount:"<<cnt<<endl;
    			A[2]=0;
		
		
		}
		
	
	
	}
    	pthread_exit((void*)0);
}


void* func(void* arg)
{

	while(1)
	{
		int count = poll(pfds,3,5000);
		if(count==-1)
		perror("poll error");
		else if(count==0)
		{printf("time out\n");}
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(cnt<3)
				{	
					
					
					if(pfds[i].revents && POLLIN)
					{	
						int nsfd;
   						 if ((nsfd = accept(pfds[i].fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    						 { 
        						 perror("accept"); 
        						 exit(EXIT_FAILURE); 
    						 } 
    						 else
    						 {
    						 	cnt++;
    						 	cout<<"\ncount:"<<cnt<<endl;
    						 	pfds[i].revents=0;
    						if(A[0]==0)
						{
							A[0]=1;
							cout<<"hi1"<<endl;
							
						}
						else if(A[1]==0)
						{
							A[1]=1;
							cout<<"hi2"<<endl;
							
						}
						else if(A[2]==0)
						{
							A[2]=1;
							cout<<"hi3"<<endl;
							
						}
    						 	
    						 
    						 
    						 
    						 
    						 }
    						
						
					}
					else
					free++;
				}
				else
				{
					cout<<"platforms are full"<<endl;
					std::this_thread::sleep_for(10000ms);
				
				}	
			}
			if(free == 3)
			cout<<"timeout"<<endl;
		}
		pfds[0].fd=sfd[0];
		pfds[0].events=POLLIN;
		pfds[0].revents=0;
		pfds[1].fd=sfd[1];
		pfds[1].events=POLLIN;
		pfds[1].revents=0;
		pfds[2].fd=sfd[2];
		pfds[2].events=POLLIN;
		pfds[2].revents=0;
	}
}



int main(int argc, const char* argv[]) 
{ 
	
    int  nsfd;
    int valread;
    int port[3];
    port[0]=8080;
    port[1]=8085;
    port[2]=7020;
    
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};    
    for(int i=0;i<3;i++)
    {
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
    	address.sin_port = htons( port[i] ); 
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
    }
   
   	
	pfds[0].fd=sfd[0];
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	pfds[1].fd=sfd[1];
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	pfds[2].fd=sfd[2];
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	pthread_create(&tid,NULL,func,NULL);
	
	pthread_create(&tid0,NULL,func1,NULL);
	pthread_create(&tid1,NULL,func2,NULL);
	pthread_create(&tid2,NULL,func3,NULL);
	pthread_join(tid,NULL);
	
	pthread_join(tid0,NULL);
	
	pthread_join(tid1,NULL);
	
	pthread_join(tid2,NULL);
	while(1);
	
	close(sfd[0]);
	close(sfd[1]);
	close(sfd[2]);
	return 0;
}









/*

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include<bits/stdc++.h>
using namespace std;
static int cnt=0;
int A[3]={0};
int sfd[3];
void* func1(void * arg)
{
	cout<<"func1"<<endl;
	int * fd=(int *)arg;
	int nsfd=*fd;
    	char buffer[1024]; 
    	cout<<nsfd<<endl;
    	cout<<"train reached platform";
    	sleep(1);
    	cnt--;
    	A[0]=0;
    	pthread_exit((void*)0);
}
void* func2(void * arg)
{
	cout<<"func2"<<endl;
	int * fd=(int *)arg;
	int nsfd=*fd;
    	char buffer[1024]; 
    	cout<<nsfd<<endl;
    	cout<<"train reached platform";
    	sleep(1);
    	cnt--;
    	A[1]=0;
    	pthread_exit((void*)0);
}
void* func3(void * arg)
{
	cout<<"func3"<<endl;
	int * fd=(int *)arg;
	int nsfd=*fd;
    	char buffer[1024]; 
    	cout<<nsfd<<endl;
    	cout<<"train reached platform";
    	sleep(1);
    	cnt--;
    	A[2]=0;
    	pthread_exit((void*)0);
}
int main(int argc, const char* argv[]) 
{ 
	
    int  nsfd;
    int valread;
    int port[3];
    port[0]=8080;
    port[1]=8085;
    port[2]=7020;
    pthread_t tid0,tid1,tid2;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};    
    for(int i=0;i<3;i++)
    {
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
    	address.sin_port = htons( port[i] ); 
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
    }
   
   	struct pollfd pfds[3];
	pfds[0].fd=sfd[0];
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	pfds[1].fd=sfd[1];
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	pfds[2].fd=sfd[2];
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	while(1)
	{
		int count = poll(pfds,3,5000);
		if(count==-1)
		perror("poll error");
		else if(count==0)
		{printf("time out\n");}
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(cnt<3)
				{	
					cnt++;
					cout<<cnt<<endl;
					if(pfds[i].revents && POLLIN)
					{	
   						 if ((nsfd = accept(pfds[i].fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    						 { 
        						 perror("accept"); 
        						 exit(EXIT_FAILURE); 
    						 } 
    						pfds[i].revents=0;
    						if(A[0]==0)
						{
							A[0]=1;
							cout<<"hi1"<<endl;
							pthread_create(&tid0,NULL,func1,&nsfd);
							
							pthread_join(tid0,NULL);
						}
						else if(A[1]==0)
						{
							A[1]=1;
							cout<<"hi2"<<endl;
							pthread_create(&tid1,NULL,func2,&nsfd);
							
							pthread_join(tid1,NULL);
						}
						else if(A[2]==0)
						{
							A[2]=1;
							cout<<"hi3"<<endl;
							pthread_create(&tid2,NULL,func3,&nsfd);
							
							pthread_join(tid2,NULL);
						}
						
					}
					else
					free++;
				}
				else
				{
					cout<<"platforms are full"<<endl;
				
				}	
			}
			if(free == 3)
			cout<<"timeout"<<endl;
		}
		pfds[0].fd=sfd[0];
		pfds[0].events=POLLIN;
		pfds[0].revents=0;
		pfds[1].fd=sfd[1];
		pfds[1].events=POLLIN;
		pfds[1].revents=0;
		pfds[2].fd=sfd[2];
		pfds[2].events=POLLIN;
		pfds[2].revents=0;
	}
	close(sfd[0]);
	close(sfd[1]);
	close(sfd[2]);
	return 0;
}


*/

