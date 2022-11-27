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
	char buffer[120];
	int fp=open("test.cpp",O_RDONLY);
	int count;
	cout<<"Hello"<<endl;
	while((count=read(fp,buffer,120))>0)
	{
		string s(buffer);
		cout<<s.substr(0,count);
		fd=open("Hero",O_WRONLY);
		write(fd,buffer,count);
		close(fd);
	}
	close(fp);
	mkfifo("code",0666);
    	fd=open("code",O_RDONLY);
    	char buff[100];
    	count=read(fd,buff,100);
    	string s(buff);
    	cout<<s.substr(0,count)<<endl;
    	close(fd);
	return 0;
}
