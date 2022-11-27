#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
using namespace std;
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
		read(rfd,buffer1,1024);
		dup2(11,1);
		int i;
		memset(buffer2,0,sizeof(buffer2));
		for( i=0;i<1024;i++)
		{
			if(buffer1[i]!='\0')
			{
				buffer2[i]=buffer1[i]-32;	
			}
			else
			break;
		}
		write(wfd,buffer2,1024);
	}
	close(rfd);
	close(wfd);
	
}
