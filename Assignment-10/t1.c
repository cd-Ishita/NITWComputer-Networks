#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#define PORT 6060
int main(){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from train1";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
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

    printf("Train 1 is connected\n");
    char buf[2048];
    read( sock , buf, 1024);
    int pid = 0;
    int i=0;
    while(buf[i] != '\0'){
        pid = pid*10 + (int)buf[i] - 48;
        i++;
    }
	printf("Pid of platform process is: %d", pid);

	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	kill(pid, SIGUSR2);
	return 0;
}