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
	struct pollfd pfds[3];
	cout<<"Enter Value for P1 : "<<endl;
	FILE *fd1=popen("./p1","r");
	wait(NULL);
	pfds[0].fd=fileno(fd1);
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	cout<<"Enter Value for P2 : "<<endl;
	FILE *fd2=popen("./p2","r");
	wait(NULL);
	pfds[1].fd=fileno(fd2);
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	cout<<"Enter Value for P3 : "<<endl;
	FILE *fd3=popen("./p3","r");
	wait(NULL);
	pfds[2].fd=fileno(fd3);
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	cout<<"Poling is Ready to start"<<endl;
	while(1)
	{
		int count = poll(pfds,3,500);
		cout<<count<<endl;
		if(count==-1)
		perror("poll error");
		else if(count==0)
		printf("time out\n");
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(pfds[i].revents && POLLIN && (c=read(pfds[i].fd,ch,120))>0)
				{	
					write(1,ch,c);
					cout<<"\t From Process "<<i+1<<endl;
					pfds[i].revents=0;
				}
				else
				free++;
			}
			if(free == 3)
			break;
		}
	cout<<"Enter Value for P1 : "<<endl;
	FILE *fd1=popen("./p1","r");
	wait(NULL);
	pfds[0].fd=fileno(fd1);
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	cout<<"Enter Value for P2 : "<<endl;
	FILE *fd2=popen("./p2","r");
	wait(NULL);
	pfds[1].fd=fileno(fd2);
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	cout<<"Enter Value for P3 : "<<endl;
	FILE *fd3=popen("./p3","r");
	wait(NULL);
	pfds[2].fd=fileno(fd3);
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	}
	
	pclose(fd1);
	pclose(fd2);
	pclose(fd3);
	return 0;
}
