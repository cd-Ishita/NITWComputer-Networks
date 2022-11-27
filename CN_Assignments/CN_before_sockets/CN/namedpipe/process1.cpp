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
		char ch1[1024],ch2[1024];
		fd=open("Hero",O_WRONLY);
		fgets(ch1, 80, stdin);
		write(fd, ch1, strlen(ch1)+1); 
        	close(fd); 
        	fd = open("Hero", O_RDONLY); 
        	read(fd, ch2, 1024); 
        	printf("User2: %s\n", ch2); 
        	close(fd); 
	}
	return 0;
}
