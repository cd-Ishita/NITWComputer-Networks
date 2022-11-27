#define __USE_BSD	
#define __FAVOR_BSD	
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include <string.h>
#include <fcntl.h> 
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h> 
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    // send to all receivers  
    int rsfd;
    struct sockaddr_in addr, cliaddr;
    int len = sizeof(cliaddr);
    if ((rsfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(rsfd, (const struct sockaddr *)&addr,
         sizeof(addr));

    char *str = "Hello World into this tatti creation!!" ;
    sendto(rsfd, str,sizeof(str), 0, (const struct sockaddr *)&addr, sizeof(addr));

    return 0 ;
}