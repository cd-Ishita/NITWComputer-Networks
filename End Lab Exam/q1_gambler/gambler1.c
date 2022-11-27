#include <pcap.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
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
#include <netinet/in_systm.h>
//#include <linux/ip.h>
//#include <linux/udp.h>
#include<string.h>
#include <netinet/if_ether.h> 
#include<net/ethernet.h>
/*for ethernet*/
#define SNP_LEN 1024
#define CMD "udp and src host %s and src port %d"
void my_callback(u_char *args,const struct pcap_pkthdr *header,const u_char *packet);
int main(int argc,char *arv[]){
	char *dev=NULL;
	char errbuf[PCAP_ERRBUF_SIZE];		
	pcap_t *handle;	
	char filter[]="";
	struct bpf_program fp;			
	uint32_t net,mask;
	int np=10;
	
	dev=pcap_lookupdev(errbuf);
	pcap_lookupnet(dev,&net,&mask,errbuf);
	
	printf("Device: %s\n", dev);
	printf("Number of packets: %d\n", np);
	printf("Filter expression: %s\n", filter);	

	handle=pcap_open_live(dev,SNP_LEN,1,1000,errbuf);
	
	if (pcap_datalink(handle) != DLT_EN10MB) 
	{
	 	fprintf(stderr, "%s is not an Ethernet\n", dev);
		exit(1);
	}
	pcap_compile(handle,&fp,filter,0,net);
	pcap_setfilter(handle,&fp);
	pcap_loop(handle,np,my_callback,NULL);
	
	pcap_freecode(&fp);
	pcap_close(handle);
		
	printf("Capture Complete\n");
	return 0;
}
void PrintData (const u_char * data , int Size){
   //u_char *ptr=(u_char *)data;
 //  const char* S1 = reinterpret_cast<const char*>(data);
  // fprintf(logfile,"%s\n",S1);
    int i , j;
    for(i=0 ; i < Size ; i++){
        if( i!=0 && i%16==0){
            printf("         ");
            for(j=i-16 ; j<i ; j++){
                if(data[j]>=32 && data[j]<=128)
                    printf("%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else printf("."); //otherwise print a dot
            }
            printf("\n");
        } 
         
        if(i%16==0) printf("   ");
            printf(" %02X",(unsigned int)data[i]);
                 
        if( i==Size-1) {
            for(j=0;j<15-i%16;j++) {
              printf("   "); //extra spaces
            }
             
            printf("         ");
             
            for(j=i-i%16 ; j<=i ; j++){
                if(data[j]>=32 && data[j]<=128) {
                  printf("%c",(unsigned char)data[j]);
                }
                else{
                  printf(".");
                }
            }
             
            printf("\n" );
        }
    }
}

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

struct sockaddr_in source,dest;
void print_ip_header(const u_char * Buffer, int Size){
    // print_ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
     
    printf("\n");
    printf("IP Header\n");
    printf("       |-IP Version        : %d\n",(unsigned int)iph->version);
    printf("       |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
    printf("       |-Type Of Service   : %d\n",(unsigned int)iph->tos);
    printf("       |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
    printf("       |-Identification    : %d\n",ntohs(iph->id));
    printf("       |-TTL      : %d\n",(unsigned int)iph->ttl);
    printf("       |-Protocol : %d\n",(unsigned int)iph->protocol);
    printf("       |-Checksum : %d\n",ntohs(iph->check));
    printf("       |-Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
    printf("       |-Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
}

void print_tcp_packet(const u_char * Buffer, int Size){
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
    printf("\n\n**********************************************************\n\n");
    print_ip_header(Buffer,Size);
         
    printf("\n");
    printf("TCP Header\n");
    printf("       |-Source Port      : %u\n",ntohs(tcph->source));
    printf("       |-Destination Port : %u\n",ntohs(tcph->dest));
    printf("       |-Sequence Number    : %u\n",ntohl(tcph->seq));
    printf("       |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    printf("       |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    printf("       |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    printf("       |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    printf("       |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    printf("       |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    printf("       |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    printf("       |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    printf("       |-Window         : %d\n",ntohs(tcph->window));
    printf("       |-Checksum       : %d\n",ntohs(tcph->check));
    printf("       |-Urgent Pointer : %d\n",tcph->urg_ptr);
    printf("\n");
         
    printf("IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    printf("TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
         
    //printf("Data Payload\n");    
    //print_header(Buffer + header_size , Size - header_size );
                         
    printf("\n###########################################################");
}

void my_callback(u_char *args,const struct pcap_pkthdr *header,const u_char *packet){
	
    //print_tcp_packet(packet, sizeof(packet));
    const struct ether_header *ehdr;
	struct ip *iphdr;
	const struct udphdr *udp;
	
	ehdr=(struct ether_header *)packet;
	iphdr=(struct ip *)(packet+14);
	
	printf("IP Address: %s\n", inet_ntoa(iphdr->ip_src));

    printf("MAC Address: %s\n", ether_ntoa((const struct ether_addr*)ehdr->ether_shost));
    
    printf("----------------------------------------------\n\n");

    

    /*
    if(iphdr->ip_p == IPPROTO_IP){
        printf("IP\n");
        printf("Message: %s", (packet+sizeof(struct ether_header)+sizeof(struct ip)
		+sizeof(struct udphdr)));
    }
	if(iphdr->ip_p==IPPROTO_TCP)
	{
		printf("TCP\n");
 		printf("Message: %s\n",(packet+sizeof(struct ether_header)+sizeof(struct ip)
		+sizeof(struct udphdr)));	
	}
    */

	return;
}
