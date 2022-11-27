#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 9090
int main(){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "S3";
	
    bzero(&serv_addr, sizeof(serv_addr));
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	printf("Client has connected\n");
	
	socklen_t serv_len = sizeof(serv_addr);
	sendto(sock , hello , 256, 0 , (struct sockaddr*)&serv_addr, serv_len);
	printf("Client wants service from %s to\n", hello);
    char buffer1[2048] = "Hello from client2\n";
    sleep(2);
	sendto(sock , buffer1 , 2048, 0 , (struct sockaddr*)&serv_addr, serv_len);
	printf("Sent data '%s' to server\n", buffer1);
    
	close(sock);
	return 0;
}