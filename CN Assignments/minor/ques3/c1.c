#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include<semaphore.h>
#include <sys/msg.h>
#include <math.h>
#include <sys/shm.h>
//compile with -lpthread -lrt
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>

int main(){
    int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Data";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(6060);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}

	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
    while(1){
        read( sock , buffer, 1024);
	    printf("%s\n",buffer );
    }

}