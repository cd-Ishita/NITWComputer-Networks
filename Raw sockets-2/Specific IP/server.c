#include <time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PORT 8000
#define MAX_LIMIT 128
#define MAXLINE 4096

void print_content(unsigned char *buf, size_t length){
    for (size_t i=0;i<length;i++){
        if (i != 0 && i % 16 == 0){
            printf("          ");
            printf("\n");
        }

        if (i % 16 == 0)
            printf("    ");
        printf("%s", buf[i]);

        if (i == (length - 1)){
            for(size_t j = 0; j < (15 - 1 % 16); j++)
                printf("    ");
            printf("          ");
            printf("\n");
        }
    }
}



int main(){
    int sfd;
    struct sockaddr_in addr, clientAddr;
    int len = sizeof(clientAddr);
    if ((sfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sfd, (const struct sockaddr *)&addr,sizeof(addr));
    printf("Bind successful!\n");
    char *buf = "Hello World!";
    sendto(sfd, buf, 2048, 0, (const struct sockaddr *)&addr, sizeof(addr));
    printf("Buffer sent!");
}