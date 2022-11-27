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

#define PORT 6060

int main(){
    
    int sfd;
    struct sockaddr_in serv_addr;
    int port = 6060;

    bzero(&serv_addr, sizeof(serv_addr));

    if((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        perror("socket\n");
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t serv_len = sizeof(serv_addr);

    char buffer[2048] = "1";
    sendto(sfd, buffer, 256, 0, (struct sockaddr*)&serv_addr, serv_len);
    char buffer1[2048] = "hello from client1";
    sendto(sfd, buffer1, 256, 0, (struct sockaddr*)&serv_addr, serv_len);
}