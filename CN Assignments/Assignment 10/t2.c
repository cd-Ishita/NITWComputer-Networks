#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#define PORT 7070
int main(){
	printf("-------- Welcome to Train 2---------\n");
	printf("I am trying to reach Station Master\n");
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "This is seating chart of Train 2\n";
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
	printf("Station Master has given me connection!\n");
    printf("Train 2 is connected to a platform\n");
    char buf[2048];
    read( sock , buf, 1024);
    int pid = 0;
    int i=0;
    while(buf[i] != '\0'){
        pid = pid*10 + (int)buf[i] - 48;
        i++;
    }
	printf("Pid of platform process is: %d\n", pid);

	send(sock , hello , strlen(hello) , 0 );
	printf("Seating chart sent\n");
	kill(pid, SIGUSR2);
    printf("Informed platform that I have left\n");
	return 0;
}