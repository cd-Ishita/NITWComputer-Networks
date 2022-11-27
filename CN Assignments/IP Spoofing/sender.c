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

// Returns host information corresponding to host name
void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}
  
// Converts space-delimited IPv4 addresses
// to dotted-decimal format
void checkIPbuffer(char *IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
}

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
#include <netinet/udp.h>
#include <netinet/ip.h>

int port = 51720;

int main() {
    int rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if(rsfd < 0) {
        perror("rsfd");
        exit(1);
    }

    int one = 1;
    if(setsockopt(rsfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char packet[8192], *payload;

    struct iphdr *iph = (struct iphdr *)packet;
    struct tcphdr *tcph = (struct tcphdr *)(packet + sizeof(struct iphdr));
    //struct udphdr *udph = (struct udphdr *)(packet + sizeof(struct iphdr));

    memset(packet, 0, 8192);
    payload = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);
    //payload = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(payload, "Ishita");

    //fill ip header
    iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + strlen(payload);
	iph->id = htonl(54321);
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;
	iph->saddr = inet_addr("12.34.56.78");
	iph->daddr = saddr.sin_addr.s_addr;
    

    //ip checksum
    iph->check = chksum((unsigned short *) packet, iph->tot_len);

    //fill tcp header
    tcph->source = htons(12345);
	tcph->dest = htons(port);
	tcph->seq = 0;
	tcph->ack_seq = 0;
	tcph->doff = 0;
	tcph->fin = 0;
	tcph->syn = 1;
	tcph->rst = 0;
	tcph->psh = 0;
	tcph->ack = 0;
	tcph->urg = 0;
	tcph->window = htonl(65535);
	tcph->check = 0;
	tcph->urg_ptr = 0;
    char hostbuffer[256];
        char *IPbuffer;
        struct hostent *host_entry;
    
    
        // To retrieve host information
        host_entry = gethostbyname(hostbuffer);
        checkHostEntry(host_entry);
    
        // To convert an Internet network address into ASCII string
        IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    if(sendto(rsfd, packet, iph->tot_len, 0,(struct sockaddr *)&saddr, sizeof(saddr)) < 0){
        perror("sendto failed");
    }
    else{
        char hostbuffer[256];
        char *IPbuffer;
        struct hostent *host_entry;
    
    
        // To retrieve host information
        host_entry = gethostbyname(hostbuffer);
        checkHostEntry(host_entry);
    
        // To convert an Internet network address into ASCII string
        IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
  
        printf("Packet Sent.\n Original IP: %s\n", IPbuffer);
    }
}