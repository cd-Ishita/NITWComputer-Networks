#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <bits/stdc++.h>
#include <sys/wait.h>
using namespace std;
#define PORT 8080 
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
int sfd, nsfd1,nsfd2,nsfd3;
void * f1(void *arg)
{
	if ((nsfd1 = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	int nsfd=nsfd1;
	while(1)
	{
		char buffer[1024] = {0}; 
		recv( nsfd , buffer, 1024,0); 
		cout<<buffer<<endl;
		string s="";
		cout<<"enter1"<<endl;
		cin>>s;
		char *hello = &s[0]; 
		send(nsfd, hello , s.length() , 0 ); 
	}
}
void * f2(void *arg)
{
	if ((nsfd2 = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	int nsfd=nsfd2;
	while(1)
	{
		char buffer[1024] = {0}; 
		recv( nsfd , buffer, 1024,0); 
		cout<<buffer<<endl;
		string s="";
		cout<<"enter2"<<endl;
		cin>>s;
		char *hello = &s[0]; 
		send(nsfd, hello , s.length() , 0 ); 
	}
}
void * f3(void *arg)
{
	if ((nsfd3 = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	int nsfd=nsfd3;
	while(1)
	{
		char buffer[1024] = {0}; 
		recv( nsfd , buffer, 1024,0); 
		cout<<buffer<<endl;
		string s="";
		cout<<"enter3"<<endl;
		cin>>s;
		char *hello = &s[0]; 
		send(nsfd, hello , s.length() , 0 ); 
	}
}

int main(int argc, char const *argv[]) 
{ 
	pthread_t t1,t2,t3;
	
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	
	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	if (bind(sfd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(sfd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	pthread_create(&t1,NULL,f1,NULL);
	pthread_create(&t2,NULL,f2,NULL);
	pthread_create(&t3,NULL,f3,NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	 
	
	while(1)
	{}
	
	/*
	
	c=fork();
	if(c>0)
	{
		close(nsfd1);
		wait(NULL);
    	//	sleep(5);
		if ((nsfd2 = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		c=fork();
		if(c>0)
		{
			close(nsfd2);
	    	//	sleep(5);
	    		wait(NULL);
	    		if ((nsfd3 = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			c=fork();
			if(c>0)
			{
				close(nsfd3);
				wait(NULL);
		    	//	sleep(5);
			}
			else
			{	
				close(sfd);
				pthread_join(t3,NULL);
				
				
			}	
		}
		else
		{
			close(sfd);
			pthread_join(t2,NULL);
			
	
		}
	}
	else
	{
		close(sfd);
		pthread_join(t1,NULL);
		
		
	
	}
	
	*/
	
	return 0; 
}

