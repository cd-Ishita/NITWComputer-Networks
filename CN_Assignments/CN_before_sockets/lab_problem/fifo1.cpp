#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include<iostream>
using namespace std;
int main()
{
	int fd;
	mkfifo("Hero",0666);
	while(1)
	{
		char ch1[1024]={'\0'};
		fd=open("Hero1",O_WRONLY);
		cout<<"enter message:"<<endl;
		fgets(ch1, 1024, stdin);
		if(ch1[0]!='\0')
		write(fd, ch1, strlen(ch1)+1); 
        	close(fd); 
        	
	}
	return 0;
}
