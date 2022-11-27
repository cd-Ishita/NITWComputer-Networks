#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include<arpa/inet.h>
#include<poll.h>

int open_socket(int port_num){
    int sock = 0, valread;
	struct sockaddr_in serv_addr;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_num);
	
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
    printf("Server 3 has connected to F\n");
    return sock;
}

int main(){
    char buf[2048];
    read(0, buf,2048);

    int port_num = 0;
    int i=0;
    while(buf[i] != '\0'){
        port_num = port_num*10 + (int)buf[i] - 48;
        i++;
    }

    int sfd = open_socket(port_num);

    while(1){
        char buffer[2048];
        read(sfd, buffer, 2048);
        printf("Server 3 is in service and got data %s\n", buffer);
        sleep(2);
    }
}