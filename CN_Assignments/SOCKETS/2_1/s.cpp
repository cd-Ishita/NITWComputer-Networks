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
int main(int argc, char const *argv[]) 
{ 
	int sfd, nsfd, valread; 
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
	int c;
	if ((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
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
		if ((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
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
	    		if ((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
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
				
				char buffer[1024] = {0}; 
				close(sfd);
				dup2(0,65);
				dup2(1,66);
				dup2(nsfd,0);
				dup2(nsfd,1);
				execv("./s3",NULL);
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
			char buffer[1024] = {0}; 
			close(sfd);
			dup2(0,65);
			dup2(1,66);
			dup2(nsfd,0);
			dup2(nsfd,1);
			execv("./s2",NULL);
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
		char buffer[1024] = {0}; 
		close(sfd);
		dup2(0,65);
		dup2(1,66);
		dup2(nsfd,0);
		dup2(nsfd,1);
		execv("./s1",NULL);
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

