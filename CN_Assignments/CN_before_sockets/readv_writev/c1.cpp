#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<iostream>

#include<sys/uio.h>
using namespace std;

void * writethread(void *arg)
{
	while(1)
	{
		struct iovec iov[1];
	//	char  buff[2]={(char*)"hi",(char*)"hello"};
		for(int i=0;i<1;i++)
		{
			cout<<"enter message: "<<i+1<<" ";
			string s="";
			getline(cin,s);
			s="C1:"+s;
			char s1[100];
			strcpy(s1,s.c_str());
			
			iov[i].iov_base=s1;
		//	cout<<"qwert1:"<<iov[i].iov_base<<endl;
			iov[i].iov_len=strlen(s1);
		
		}
		int fd=open("server",O_WRONLY);
		
		writev(fd,iov,1);
		close(fd);
	}
}
int main()
{
	pthread_t t1,t2;
	mkfifo("server",0666);
	mkfifo("client1",0666);
//	pthread_create(&t1,NULL,readthread,NULL);
	pthread_create(&t2,NULL,writethread,NULL);
//	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	
	
}
