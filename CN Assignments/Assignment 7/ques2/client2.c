#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
 

int main(int argc, char *argv[]){
    int sfd;
    struct sockaddr_in server_addr;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1){
        perror("socket");
    }

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(3490);

    int connect_status = connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int x = getsockname(sfd, &address, &address_len);

    printf("Local IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("Local port is: %d\n", (int) ntohs(address.sin_port));

    
    char *buf = "Hello from Client 2!";
    send(sfd, buf, 2048, 0);
    printf("client2: sent '%s'\n",buf);


    char message_recv[2048];
    int bytes_recv = recv(sfd, message_recv, 2047, 0);
    message_recv[bytes_recv] = '\0';
    printf("Message received is: %s", message_recv);


    close(sfd);

    return 0;
}