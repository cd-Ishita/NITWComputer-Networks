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

#define PORT "3590" // the port client will be connecting to 
#define MAX_SIZE 1024

int open_socket(){
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *servinfo;
    int rv;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        printf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    struct addrinfo *p;
    int sfd;
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            printf("client: socket");
            continue;
        }

        if (connect(sfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sfd);
            continue;
            }

        break;
    }

    if (p == NULL) {
        printf(stderr, "client: failed to connect\n");
        return 2;
    }
    
    return sfd;
}

int main(int argc, char *argv[]){
    int sfd = open_socket();

    /*
    struct sockaddr_in server_addr;

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1){
        printf("socket connection error");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3590);

    int connect_status = connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(connect_status == -1){
        printf("connection error");
        return 1;
    }

*/
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int x = getsockname(sfd, &address, &address_len);

    printf("Local IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("Local port is: %d\n", (int) ntohs(address.sin_port));

    //File to be sent is opened
    FILE *fp;
    char *filename = "P.cpp";
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("[-]Error in reading file.");
        printf("error in file reading");
        exit(1);
    }

    //send the file
    int n;
    char data[MAX_SIZE] = {0};

    while(fgets(data, MAX_SIZE, fp) != NULL) {
        if (send(sfd, data, sizeof(data), 0) == -1) {
            perror("[-]Error in sending file.");
            printf("error");
            exit(1);
        }
        //printf("%s", data);
        bzero(data, MAX_SIZE);
    }
    printf("File is sent from client.\n");
    
    close(sfd);
    printf("Enter char: ");
    int d;
    scanf("%d", d);
    int sfd1 = open_socket();
    
    char message_recv[2048];
    int bytes_recv = recv(sfd1, message_recv, 2047, 0);
    message_recv[bytes_recv] = '\0';
    printf("Client received: %s\n", message_recv);
    
    //close(sfd);

    return 0;
}