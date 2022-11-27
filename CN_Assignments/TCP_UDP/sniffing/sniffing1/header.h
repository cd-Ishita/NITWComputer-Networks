#include<bits/stdc++.h>

#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<cstdlib>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>


using namespace std;

struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j;
FILE * logfile; //to store the output

// struct ethhdr
// {
//   uint8_t  h_dest[ETH_ALEN];	/* destination eth addr	*/
//   uint8_t  h_source[ETH_ALEN];	 source ether addr	
//   uint16_t h_proto;		        /* packet type ID field	*/
// }__attribute__((packed));

struct myipheader
 {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
 };

struct tcphdr
{
	__extension__ union
	{
		// struct
		// {
		// 	uint16_t th_sport;	/* source port */
		// 	uint16_t th_dport;	/* destination port */
		// 	tcp_seq th_seq;		/* sequence number */
		// 	tcp_seq th_ack;		/* acknowledgement number */
		// 	# if __BYTE_ORDER == __LITTLE_ENDIAN
		// 	uint8_t th_x2:4;	/* (unused) */
		// 	uint8_t th_off:4;	/* data offset */
		// 	# endif
		// 	# if __BYTE_ORDER == __BIG_ENDIAN
		// 	uint8_t th_off:4;	/* data offset */
		// 	uint8_t th_x2:4;	/* (unused) */
		// 	# endif
		// 	uint8_t th_flags;
		// 	# define TH_FIN	0x01
		// 	# define TH_SYN	0x02
		// 	# define TH_RST	0x04
		// 	# define TH_PUSH	0x08
		// 	# define TH_ACK	0x10
		// 	# define TH_URG	0x20
		// 	uint16_t th_win;	/* window */
		// 	uint16_t th_sum;	/* checksum */
		// 	uint16_t th_urp;	/* urgent pointer */
		// };
		struct
		{
			uint16_t source;
			uint16_t dest;
			uint32_t seq;
			uint32_t ack_seq;
			# if __BYTE_ORDER == __LITTLE_ENDIAN
			uint16_t res1:4;
			uint16_t doff:4;
			uint16_t fin:1;
			uint16_t syn:1;
			uint16_t rst:1;
			uint16_t psh:1;
			uint16_t ack:1;
			uint16_t urg:1;
			uint16_t res2:2;
			# elif __BYTE_ORDER == __BIG_ENDIAN
			uint16_t doff:4;
			uint16_t res1:4;
			uint16_t res2:2;
			uint16_t urg:1;
			uint16_t ack:1;
			uint16_t psh:1;
			uint16_t rst:1;
			uint16_t syn:1;
			uint16_t fin:1;
			# else
			#  error "Adjust your <bits/endian.h> defines"
			# endif
			uint16_t window;
			uint16_t check;
			uint16_t urg_ptr;
		};
	};
};


// struct udphdr
// {
//   __extension__ union
//   {
//     struct
//     {
//       uint16_t uh_sport;	/* source port */
//       uint16_t uh_dport;	/* destination port */
//       uint16_t uh_ulen;		 udp length 
//       uint16_t uh_sum;		/* udp checksum */
//     };
//     struct
//     {
//       uint16_t source;
//       uint16_t dest;
//       uint16_t len;
//       uint16_t check;
//     };
//   };
// };

unsigned short csum(unsigned short *ptr,int nbytes) 
{
	register long sum;
	unsigned short oddbyte;
	register short answer;

	sum=0;
	while(nbytes>1) {
		sum+=*ptr++;
		nbytes-=2;
	}
	if(nbytes==1) {
		oddbyte=0;
		*((u_char*)&oddbyte)=*(u_char*)ptr;
		sum+=oddbyte;
	}

	sum = (sum>>16)+(sum & 0xffff);
	sum = sum + (sum>>16);
	answer=(short)~sum;
	
	return(answer);
}

void print_ethernet_header(const u_char *Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    printf( "\n");
    printf( "Ethernet Header\n");
    printf( "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    printf( "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    printf( "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}

void PrintData (const u_char * data , int Size)
{
   //u_char *ptr=(u_char *)data;
 //  const char* S1 = reinterpret_cast<const char*>(data);
  // printf(logfile,"%s\n",S1);
    int i , j;
    for(i=0 ; i < Size ; i++)
    {
        if( i!=0 && i%16==0)   //if one line of hex printing is complete...
        {
            printf( "         ");
            for(j=i-16 ; j<i ; j++)
            {
                if(data[j]>=32 && data[j]<=128)
                    printf( "%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                else printf( "."); //otherwise print a dot
            }
            printf( "\n");
        } 
         
        if(i%16==0) printf( "   ");
            printf( " %02X",(unsigned int)data[i]);
                 
        if( i==Size-1)  //print the last spaces
        {
            for(j=0;j<15-i%16;j++) 
            {
              printf( "   "); //extra spaces
            }
             
            printf( "         ");
             
            for(j=i-i%16 ; j<=i ; j++)
            {
                if(data[j]>=32 && data[j]<=128) 
                {
                  printf( "%c",(unsigned char)data[j]);
                }
                else
                {
                  printf( ".");
                }
            }
             
            printf(  "\n" );
        }
    }
}



void print_ip_header(const u_char * Buffer, int Size)
{
    print_ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
     
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
 
void print_tcp_packet(const u_char * Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
     
    printf( "\n\n***********************TCP Packet*************************\n");  
         
    print_ip_header(Buffer,Size);
         
    printf( "\n");
    printf( "TCP Header\n");
    printf( "   |-Source Port      : %u\n",ntohs(tcph->source));
    printf( "   |-Destination Port : %u\n",ntohs(tcph->dest));
    printf( "   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    printf( "   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    printf( "   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
    printf( "   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    printf( "   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    printf( "   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    printf( "   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    printf( "   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    printf( "   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    printf( "   |-Window         : %d\n",ntohs(tcph->window));
    printf( "   |-Checksum       : %d\n",ntohs(tcph->check));
    printf( "   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    printf( "\n");
    printf( "                        DATA Dump                         ");
    printf( "\n");
         
    printf( "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    printf( "TCP Header\n");
    PrintData(Buffer+iphdrlen,tcph->doff*4);
         
    printf( "Data Payload\n");    
    PrintData(Buffer + header_size , Size - header_size );
                         
    printf( "\n###########################################################");
}
 
void print_udp_packet(const u_char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
    printf( "\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(Buffer,Size);           
     
    printf( "\nUDP Header\n");
    printf( "   |-Source Port      : %d\n" , ntohs(udph->source));
    printf( "   |-Destination Port : %d\n" , ntohs(udph->dest));
    printf( "   |-UDP Length       : %d\n" , ntohs(udph->len));
    printf( "   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    printf( "\n");
    printf( "IP Header\n");
    PrintData(Buffer , iphdrlen);
         
    printf( "UDP Header\n");
    PrintData(Buffer+iphdrlen , sizeof udph);
         
    printf( "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , Size - header_size);
     
    printf( "\n###########################################################");
}
 
// void print_icmp_packet(const u_char * Buffer , int Size)
// {
//     unsigned short iphdrlen;
     
//     struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
//     iphdrlen = iph->ihl * 4;
     
//     struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
//     int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
//     printf( "\n\n***********************ICMP Packet*************************\n"); 
     
//     print_ip_header(Buffer , Size);
             
//     printf( "\n");
         
//     printf( "ICMP Header\n");
//     printf( "   |-Type : %d",(unsigned int)(icmph->type));
             
//     if((unsigned int)(icmph->type) == 11)
//     {
//         printf( "  (TTL Expired)\n");
//     }
//     else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
//     {
//         printf( "  (ICMP Echo Reply)\n");
//     }
     
//     printf( "   |-Code : %d\n",(unsigned int)(icmph->code));
//     printf( "   |-Checksum : %d\n",ntohs(icmph->checksum));
//     //printf( "   |-ID       : %d\n",ntohs(icmph->id));
//     //printf( "   |-Sequence : %d\n",ntohs(icmph->sequence));
//     printf( "\n");
 
//     printf( "IP Header\n");
//     PrintData(Buffer,iphdrlen);
         
//     printf( "UDP Header\n");
//     PrintData(Buffer + iphdrlen , sizeof icmph);
         
//     printf( "Data Payload\n");    
     
//     //Move the pointer ahead and reduce the size of string
//     PrintData(Buffer + header_size , (Size - header_size) );
     
//     printf( "\n###########################################################");
// }
 



void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    int size = header->len;
     
    //Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
        case 1:  //ICMP Protocol
            ++icmp;
            // print_icmp_packet( buffer , size);
            break;
         
        case 2:  //IGMP Protocol
            ++igmp;
            break;
         
        case 6:  //TCP Protocol
            ++tcp;
            print_tcp_packet(buffer , size);
            break;
         
        case 17: //UDP Protocol
            ++udp;
            print_udp_packet(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    }
    printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r", tcp , udp , icmp , igmp , others , total);
}

// char* itoa(int num, char* str, int base)
// {
//     int i = 0;
//     bool isNegative = false;
  
//     /* Handle 0 explicitely, otherwise empty string is printed for 0 */
//     if (num == 0)
//     {
//         str[i++] = '0';
//         str[i] = '\0';
//         return str;
//     }
  
//     // In standard itoa(), negative numbers are handled only with 
//     // base 10. Otherwise numbers are considered unsigned.
//     if (num < 0 && base == 10)
//     {
//         isNegative = true;
//         num = -num;
//     }
  
//     // Process individual digits
//     while (num != 0)
//     {
//         int rem = num % base;
//         str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
//         num = num/base;
//     }
  
//     // If number is negative, append '-'
//     if (isNegative)
//         str[i++] = '-';
  
//     str[i] = '\0'; // Append string terminator
  
//     // Reverse the string
//     reverse(str, i);
  
//     return str;
// }
