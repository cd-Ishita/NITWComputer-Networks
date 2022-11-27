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

void print_header(unsigned char *buf, size_t length){
    for (size_t i = 0; i < length; i++){
        if (i != 0 && i % 16 == 0){
            printf("          ");
            printf("\n");
        }

        if (i % 16 == 0)
            printf("IP header : ");
        printf("%c",buf[i]);

        if (i == (length - 1)){
            for (size_t j = 0; j < (15 - 1 % 16); j++)
                printf("    ");
            printf("          ");
            printf("\n");
        }
    }
}

void print_ipheader(struct iphdr *ip){
    printf("------------------------\n");
    printf("Printing IP header....\n");
    printf("IP version: %d \n",  (unsigned int)ip->version );
    printf("IP header length: %d \n" , (unsigned int)ip->ihl );
    printf("Type of service: %d \n" ,(unsigned int)ip->tos);
    printf("Total ip packet length: %d \n" ,ntohs(ip->tot_len) );
    printf("Packet id: %d \n" ,ntohs(ip->id) );
    printf("Time to leave: %d \n" ,(unsigned int)ip->ttl );
    printf("Protocol: %d \n" ,(unsigned int)ip->protocol );
    printf("Check: %d \n" ,ip->check );
    printf("Source ip: %s \n" ,inet_ntoa(*(struct in_addr *)&ip->saddr) );
    printf("Destination ip: %s \n" ,inet_ntoa(*(struct in_addr *)&ip->daddr) );
    printf("End of IP header\n");
    printf("------------------------\n");
    printf("Payload: ");
}

#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
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

    while(1) {
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
}