#include<bits/stdc++.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/in_systm.h>
#include <netinet/ip_icmp.h>
#include <netinet/ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <poll.h>
#define __USE_BSD
#define __FAVOR_BSD	
#define BUFSIZE 1024
using namespace std;

FILE *logfile;
struct sockaddr_in source,dest;

void print_ip_header(const u_char * Buffer)
{
    // print_ethernet_header(Buffer , Size);
    // unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)Buffer ;
    int iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf( "\n");
    printf( "IP Header\n");
    printf( "   |-IP Version        : %d\n",(unsigned int)iph->version);
    printf( "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf( "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf( "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf( "   |-Identification    : %d\n",ntohs(iph->id));
    printf( "   |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf( "   |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf( "   |-Checksum : %d\n",ntohs(iph->check));
    printf( "   |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    printf( "   |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
    
}
void print_ip_packet(const u_char * Buffer){
        struct iphdr *iph = (struct iphdr *)Buffer ;
        int iphdrlen =iph->ihl*4;
        print_ip_header(Buffer);
        cout<<Buffer+iphdrlen<<endl;

}
 
void print_tcp_packet(const u_char * Buffer)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *) Buffer ;
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen );
             
    int header_size =  iphdrlen + tcph->doff*4;
     
    printf("\n\n***********************TCP Packet*************************\n");  
         
    print_ip_header(Buffer);
         
    printf("\n");
    printf("TCP Header\n");
    printf("   |-Source Port      : %u\n",ntohs(tcph->source));
    printf("   |-Destination Port : %u\n",ntohs(tcph->dest));
    printf("   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    printf("   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    printf("   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    printf("   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    printf("   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    printf("   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    printf("   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    printf("   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    printf("   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    printf("   |-Window         : %d\n",ntohs(tcph->window));
    printf("   |-Checksum       : %d\n",ntohs(tcph->check));
    printf("   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    printf("\n");
    printf("                        DATA Dump                         ");
    printf("\n");
    cout<<Buffer+header_size<<endl;
    printf("\n###########################################################");
}

void print_udp_packet(const u_char *Buffer)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
    printf( "\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(Buffer);           
     
    printf( "\nUDP Header\n");
    printf( "   |-Source Port      : %d\n" , ntohs(udph->source));
    printf( "   |-Destination Port : %d\n" , ntohs(udph->dest));
    printf( "   |-UDP Length       : %d\n" , ntohs(udph->len));
    printf( "   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
     cout<<Buffer+header_size<<endl;
    printf( "\n###########################################################");
}

int main(){
	int s = socket (PF_INET, SOCK_RAW, IPPROTO_TCP);
  perror("socket:");	/* open raw socket */

  
  
  struct sockaddr_in sin;
	/* the sockaddr_in containing the dest. address is used
	in sendto() to determine the datagrams path */

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr("127.0.0.1"); 

  	/* zero out the buffer */
  
  while (1){

  		char datagram[4096];
  		cout<<"msg to be sent:";
  		cin>>datagram;
  		cout<<datagram<<endl;
      if (sendto (s,datagram,	sizeof(datagram),0, (struct sockaddr *) &sin,sizeof (sin)) > 0)	printf ("Success\n");
}
 

  return 0;


}
