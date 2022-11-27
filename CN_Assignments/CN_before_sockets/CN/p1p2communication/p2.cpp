#include<unistd.h>
#include<cstring>
#include <stdio.h> 
#include <sys/types.h>      
#include <sys/wait.h>
#include<bits/stdc++.h>
#include<thread>
#define range 1024
using namespace std;
void readthread(int fd,char buff[range],int count)
{
	read(fd,buff,count);
}
void writethread(int fd,char buff[range],int count)
{
	write(fd,buff,count);
}
int main()
{
	
	char buffer1[1024];
	char buffer2[1024];	
	int rfd,wfd;
    rfd=dup(0);
    wfd=dup(1);
	int t=2;
	while(t--)
	{
		dup2(11,1);
		dup2(10,0);
		thread child1(readthread,rfd,buffer1,range);
		child1.join();
		cout<<"Child :- Data Read : "<<buffer1<<endl;
		cout<<"child :- Enter Data : ";
		cin>>buffer2;
		thread child2(writethread,wfd,buffer2,range);
		child2.join();
		
		
		
	}
	close(rfd);
	close(wfd);
	
}
