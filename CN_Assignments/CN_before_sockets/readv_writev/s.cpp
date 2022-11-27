#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<iostream>
#include<sys/uio.h>
using namespace std;

struct iovec iov; 

/*

void *readthread(void *arg)	//reading from s
{	
	while(1)
	{
		string s="";
		cout<<"enter message in sever: "<<endl;
		getline(cin,s);
		if(s.length()>0)
		{
			int pd=open("server",O_RDONLY);
			char * buffer=&s[0];
			write(pd,buffer,s.length());
			close(pd);
		}
	}
}
*/

void *writethread(void *arg)	// writting into c1 c2 c3
{
	while(1)
	{
		struct iovec iov[1];
		string s="";
		char buff[100];
		for(int i=0;i<1;i++)
		{
			iov[i].iov_base=buff;
			iov[i].iov_len=100;
		
		}
		int fd = open("server", O_RDONLY);
		readv(fd, iov,1); 
		close(fd);
		string a(buff);
		cout<<"server1:"<<a<<endl;
	//	string b(buff[1]);
	//	cout<<"server2:"<<b<<endl;
		
		
		
		/*
		struct iovec iov1[2];
		for(int i=0;i<2;i++)
		{
		//	cout<<"enter message: "<<i+1<<" ";
			string s(buff[i]);
			s="server:"+s;
			char * s1=&s[0];
			int k=strlen(&s[0]);
			
			iov1[i].iov_base=s1;
			iov1[i].iov_len=strlen(s1);
		
		}
		
		
		
		
		
		
		
      		
 		if(1)
        	{
        		
    	   		int pd1=open("client1",O_WRONLY);
			writev(pd1,iov1,2);
			close (pd1);
			int pd2=open("client2",O_WRONLY);
			writev(pd2,iov1, 2);
			close (pd2);
			int pd3=open("client3",O_WRONLY);
			writev(pd3,iov1,2);
			close (pd3);
		}
		*/
        
		
	}
}
int main()
{
	pthread_t t1,t2;
	mkfifo("server",0666);
	mkfifo("client1",0666);
	mkfifo("client2",0666);
	mkfifo("client3",0666);
//	pthread_create(&t1,NULL,readthread,NULL);
	pthread_create(&t2,NULL,writethread,NULL);
//	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
}
