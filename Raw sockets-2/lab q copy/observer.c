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

int main(){
    int rsfd;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    if ((rsfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    int len = sizeof(addr);

    unsigned char buffer[2048];

    int packetSize = recvfrom(rsfd, (char *)buffer, 4086,0, (struct sockaddr *)&addr,(socklen_t *)&len);

    unsigned char tempBuff[4086];
    for (int i = 0; i < 4086; i++)
        tempBuff[i] = buffer[i];
    struct iphdr *iph = (struct iphdr *)buffer;
    print_ipheader(iph);

    unsigned short ip_head_len = iph->ihl * 4;
    struct tcphdr *tcp_head = (struct tcphdr *)(tempBuff + ip_head_len);
    print_header(tempBuff + ip_head_len + tcp_head->th_off * 4, (packetSize - tcp_head->th_off * 4 - iph->ihl * 4));
    memset(buffer, 0, 4086);

    return 0 ;
}