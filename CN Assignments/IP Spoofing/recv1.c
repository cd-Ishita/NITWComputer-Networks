#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stddef.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>


#define h_addr h_addr_list[0]

unsigned short chksum(void *b, int len) {    
    unsigned short *buf = b; 
    unsigned int sum = 0; 
    unsigned short result; 

    for ( sum = 0; len > 1; len -= 2 ) 
        sum += *buf++; 
    if ( len == 1 ) 
        sum += *(unsigned char*)buf; 
    sum = (sum >> 16) + (sum & 0xFFFF); 
    sum += (sum >> 16); 
    result = ~sum; 
    return result; 
}

#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define h_addr h_addr_list[0]

int main() {

    int cli_port = 51720;
    int sfd;
    char *name = "localhost";
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server_details;
    server_details = gethostbyname(name);
    
    memset(&cli_addr, 0, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_port = htons(cli_port);
    bcopy((char *) server_details->h_addr, 
         (char *) &cli_addr.sin_addr.s_addr, 
         server_details->h_length);

    //create socket
    if((sfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {
        perror("socket");
        exit(1);
    }

    //setsockopt to reuse the same address
    int opt = 1;
    if((setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))) < 0) {
        perror("setsockopt");
        exit(1);
    }

    //bind address to socket fd
    if(bind(sfd, (struct sockaddr*)&cli_addr, sizeof(cli_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    
    char buff[100];
    int len = sizeof(serv_addr);
    int r = recvfrom(sfd, buff, 100, 0, (struct sockaddr *)&serv_addr, &len);
    //extract ip header
    struct iphdr *iph = (struct iphdr*)buff;

    //convert ip address from number to . format
    char buff1[100];
    struct in_addr in;
    in.s_addr = iph->saddr;
    strcpy(buff1, inet_ntoa(in));
    
    printf("%s from : %s\n", buff + sizeof(struct iphdr) + sizeof(struct tcphdr), buff1);
    
}