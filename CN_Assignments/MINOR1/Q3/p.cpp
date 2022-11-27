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
using namespace std;
int main()
{

	
	int fd2=fileno(popen("./p2","r"));
	int fd3=fileno(popen("./p3","r"));
	int fd4=fileno(popen("./p4","r"));


	struct pollfd pfds[3];
	
	
	
	
	while(1)
	{
		pfds[0].fd=fd2;
		pfds[0].events=POLLIN;
		pfds[1].fd=fd3;
		pfds[1].events=POLLIN;
		pfds[2].fd=fd4;
		pfds[2].events=POLLIN;
		
		
		
		int count = poll(pfds,3,0);
		if(count>0)
		{
		for(int i=0;i<3;i++)
		{
			if(pfds[i].revents && POLLIN)
			{	
				dup2(0,20);
				dup2(pfds[i].fd,0);
				string s;
				getline(cin,s);
				
				cout<<s<<endl;
				pfds[i].revents=0;
				dup2(20,0);
					
			}
		
		
		
		
		}
			
		
		
		}

	}
	
	return 0;
}
