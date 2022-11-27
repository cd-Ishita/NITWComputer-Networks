#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 6060
int main(){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "S1";
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
    printf("Client has connected\n");
	
	send(sock , hello , strlen(hello) , 0 );
	printf("Client wants service from %s to\n", hello);
    char buffer1[2048] = "client1";
    sleep(2);
	send(sock, buffer1, 2048, 0);
	printf("Sent data '%s' to server\n", buffer1);
    
	char buffer[2048];
    read(sock, buffer, 2048);
	printf("Received this from server: %s\n", buffer);
	close(sock);
	return 0;
}