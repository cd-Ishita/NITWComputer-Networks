#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <bits/stdc++.h>
#include <sys/wait.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include <sys/wait.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<poll.h>
#include <unistd.h>
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
using namespace std;
#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
	int sfd1,sfd2,sfd3, nsfd, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	
	if ((sfd1 = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (setsockopt(sfd1, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( 8080 ); 
	if (bind(sfd1, (struct sockaddr *)&address, sizeof(address))<0) 
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
	if (setsockopt(sfd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( 8081 ); 
	if (bind(sfd2, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(sfd2, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	
	if ((sfd3 = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (setsockopt(sfd3, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( 8082 ); 
	if (bind(sfd3, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(sfd3, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	
	
	struct pollfd pfds[3];
		pfds[0].fd=sfd1;
		pfds[0].events=POLLIN;
		pfds[1].fd=sfd2;
		pfds[1].events=POLLIN;
		pfds[2].fd=sfd3;
		pfds[2].events=POLLIN;
		int count = poll(pfds,3,0);
	int c;
	if ((nsfd = accept(sfd1, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	c=fork();
	if(c>0)
	{
		close(nsfd);
		wait(NULL);
    	//	sleep(5);
		if ((nsfd = accept(sfd2, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 
		c=fork();
		if(c>0)
		{
			close(nsfd);
	    	//	sleep(5);
	    		wait(NULL);
	    		if ((nsfd = accept(sfd3, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE); 
			} 
			c=fork();
			if(c>0)
			{
				close(nsfd);
				wait(NULL);
		    	//	sleep(5);
			}
			else
			{
				cout<<"hi3"<<endl;
				char buffer[1024] = {0}; 
				close(sfd3);
				valread = recv( nsfd , buffer, 1024,0); 
				cout<<buffer<<endl;
				string s="";
				cout<<"enter"<<endl;
				cin>>s;
				char *hello = &s[0]; 
				send(nsfd, hello , s.length() , 0 ); 
			}	
		}
		else
		{
			cout<<"hi2"<<endl;
			char buffer[1024] = {0}; 
			close(sfd2);
			valread = recv( nsfd , buffer, 1024,0); 
			cout<<buffer<<endl;
			string s="";
			cout<<"enter"<<endl;
			cin>>s;
			char *hello = &s[0]; 
			send(nsfd, hello , s.length() , 0 ); 
	
		}
	}
	else
	{
		cout<<"hi1"<<endl;
		char buffer[1024] = {0}; 
		close(sfd1);
		valread = recv( nsfd , buffer, 1024,0); 
		cout<<buffer<<endl;
		string s="";
		cout<<"enter"<<endl;
		cin>>s;
		char *hello = &s[0]; 
		send(nsfd, hello , s.length() , 0 ); 
	
	}
	
	return 0; 
}

/*

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#define PORT 8080 
int main(int argc, char const *argv[]) 
{ 
    int sfd, nsfd, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);    
    // Creating socket file descriptor 
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(sfd, (struct sockaddr *)&address,sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(sfd,3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
    int c=1;
   // for(int i=0;i<3;i++)
    {
    if ((nsfd = accept(sfd, (struct sockaddr )&address,(socklen_t)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
   // if(c>0)
    c=fork();
    if(c>0)
    {
    	close(nsfd);
    	wait(NULL);
    	if ((nsfd = accept(sfd, (struct sockaddr )&address,(socklen_t)&addrlen))<0) 
    	{ 
        	perror("accept"); 
        	exit(EXIT_FAILURE); 
    	}
   // if(c>0)
    	c=fork();
    	if(c>0)
    	{
    		close(nsfd);
    		wait(NULL);
    		if ((nsfd = accept(sfd, (struct sockaddr )&address,(socklen_t)&addrlen))<0) 
    		{ 
        		perror("accept"); 
        		exit(EXIT_FAILURE); 
    		}
   // if(c>0)
    		c=fork();
    		if(c>0)
    		{
    			close(nsfd);
    		    	wait(NULL);
   	 	} 
    		else
    		{
    			close(sfd);
 //   	while(1)
    			{
    				char buffer[1024] = {0}; 
    				char hello[100]; 
    				recv( nsfd , buffer, 1024,0); 
    				printf("%s3\nenter:",buffer ); 
   				scanf("%s",hello);
    				send(nsfd,hello,strlen(hello),0 ); 
   				printf("Hello message sent\n"); 
    			}
    		}
   	 } 
    	else
    	{
    		close(sfd);
 //   	while(1)
    		{
    			char buffer[1024] = {0}; 
    			char hello[100]; 
    			recv( nsfd , buffer, 1024,0); 
    			printf("%s2\nenter:",buffer); 
   			scanf("%s",hello);
    			send(nsfd,hello,strlen(hello),0 ); 
   			printf("Hello message sent\n"); 
    		}
    	}
    	
    } 
    else
    {
    	close(sfd);
 //   	while(1)
    	{
    		char buffer[1024] = {0}; 
    		char hello[100]; 
    		recv( nsfd , buffer, 1024,0); 
    		printf("%s1\nenter:",buffer); 
   		scanf("%s",hello);
    		send(nsfd,hello,strlen(hello),0 ); 
   		printf("Hello message sent\n"); 
    	}
    }
    }
    return 0; 
}

*/

