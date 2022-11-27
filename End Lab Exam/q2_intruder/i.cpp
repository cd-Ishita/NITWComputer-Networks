#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>   
#include<pthread.h>  
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/select.h>
#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
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
#include<stdio.h>	//for printf
#include<string.h> //memset
#include<sys/socket.h>	//for socket ofcourse
#include<stdlib.h> //for exit(0);
#include<errno.h> //For errno - the error number
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header




/* 
	96 bit (12 bytes) pseudo header needed for udp header checksum calculation 
*/
struct pseudo_header
{
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

/*
	Generic checksum calculation function
*/
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

#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

int port = 51720;


using namespace std;

void process_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
void PrintData(const u_char *, int);
void print_udp_packet(const u_char *, int);

struct sockaddr_in source, dest;
int i, j;
int rsfd;
pcap_t *handle; // Handle of the device that shall be sniffed
int main() {

  if ((rsfd = socket(AF_INET, SOCK_RAW, 201)) < 0)
    perror("rsfd");
  int optval = 1;
  if (setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int)) < 0)
    perror("setsockopt");
  struct sockaddr_in client_addr, server_addr;
  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = inet_addr("127.0.4.4");
  socklen_t addrlen = sizeof(client_addr);

  if (bind(rsfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
    perror("bind");

  pcap_if_t *alldevsp, *device;
  

  struct bpf_program fp;
  bpf_u_int32 net;
  bpf_u_int32 mask;
  char filter[] = "src host 127.0.0.7";
  //char filter[] = "";

  char errbuf[100], *devname, devs[100][100];
  int count = 1, n;

  // get the list of available devices
  printf("Finding available devices ... ");
  if (pcap_findalldevs(&alldevsp, errbuf))
  {
    printf("Error finding devices : %s", errbuf);
    exit(1);
  }
  printf("Done");

  // Print the available devices
  printf("\nAvailable Devices are :\n");
  for (device = alldevsp; device != NULL; device = device->next)
  {
    printf("%d. %s - %s\n", count, device->name, device->description);
    if (device->name != NULL)
    {
      strcpy(devs[count], device->name);
    }
    count++;
  }

  // Ask user which device to sniff
  printf("Enter the number of the device you want to sniff : ");
  scanf("%d", &n);
  devname = devs[n];

  pcap_lookupnet(devname, &net, &mask, errbuf);

  // Open the device for sniffing
  printf("Opening device %s for sniffing ... ", devname);
  handle = pcap_open_live(devname, 65535, 1, 100, errbuf);

  if (handle == NULL)
  {
    fprintf(stderr, "Couldn't open device %s : %s\n", devname, errbuf);
    exit(1);
  }
  printf("Done\n");

  if (pcap_datalink(handle) != DLT_EN10MB)
  {
    fprintf(stderr, "%s is not an Ethernet\n", devname);
    exit(1);
  }

  if (pcap_compile(handle, &fp, filter, 0, net) == -1)
  {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter, pcap_geterr(handle));
    return (2);
  }

  if (pcap_setfilter(handle, &fp) == -1)
  {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter, pcap_geterr(handle));
    return (2);
  }

  printf("Starting loop\n");

  pcap_loop(handle, 20, process_packet, NULL);
  return 0;
}
char result[2048] = "";
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer){
    int size = header->len;

    // Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    
	switch (iph->protocol) // Check the Protocol and do accordingly...
    {

  		case 17: {
		print_udp_packet(buffer, size);

		u_char* packet;
		packet = (u_char*)malloc(sizeof(u_char)*1500); 
		struct ether_header* eth;
		eth = (struct ether_header*)packet; 
		
		eth->ether_type = htons(ETHERTYPE_IP);

		
		//Datagram to represent the packet
		char datagram[4096] , source_ip[32] , *data , *pseudogram;
		
		//zero out the packet buffer
		memset (datagram, 0, 4096);
		
		//IP header
		struct iphdr *iph = (struct iphdr *) datagram;
		
		//udp header
		struct udphdr *udph = (struct udphdr *) (datagram + sizeof (struct ip));
		struct sockaddr_in sin;
		struct pseudo_header psh;
		
		
		sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);

		//sendto(rsfd, buffer, BUFSIZ, 0, (sockaddr *)&addr, addrlen);
		
		//Data part
		data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
		strcpy(data , result);
		
		//some address resolution
		strcpy(source_ip , "127.0.0.1");
		sin.sin_family = AF_INET;
		sin.sin_port = htons(80);
		sin.sin_addr.s_addr = inet_addr ("127.0.0.7");
		
		//Fill in the IP Header
		iph->ihl = 5;
		iph->version = 4;
		iph->tos = 0;
		iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
		iph->id = htonl (54321);	//Id of this packet
		iph->frag_off = 0;
		iph->ttl = 0;
		iph->protocol = IPPROTO_UDP;
		iph->check = 0;		//Set to 0 before calculating checksum
		iph->saddr = inet_addr ( source_ip );	//Spoof the source ip address
		iph->daddr = sin.sin_addr.s_addr;
		
		//Ip checksum
		iph->check = csum ((unsigned short *) datagram, iph->tot_len);

		//iph->id = htonl(12345);
		
		//udp Header
		udph->source = htons (1234);
		udph->dest = htons (6060);
		udph->check = 0;	//leave checksum 0 now, filled later by pseudo header
		udph->len = htons(8+strlen(data));
		
		//Now the udp checksum
		psh.source_address = inet_addr( source_ip );
		psh.dest_address = sin.sin_addr.s_addr;
		psh.placeholder = 0;
		psh.protocol = IPPROTO_UDP;
		psh.udp_length = htons(sizeof(struct udphdr) + strlen(data) );
		
		int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
		pseudogram = (char *)malloc(psize);
		
		memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
		memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
		
		udph->check = csum( (unsigned short*) pseudogram , psize);
		
		//IP_HDRINCL to tell the kernel that headers are included in the packet
		int one = 1;
		const int *val = &one;
		
			if(pcap_inject(handle,packet,44)<=0){
				perror("Could not send");
			}
			cout << "Packet sent to Client\n";

			break;
		}
  	}
}
void print_udp_packet(const u_char *Buffer, int Size){

  unsigned short iphdrlen;

  struct iphdr *iph = (struct iphdr *)(Buffer + sizeof(struct ethhdr));
  iphdrlen = iph->ihl * 4;

  struct udphdr *udph = (struct udphdr *)(Buffer + iphdrlen + sizeof(struct ethhdr));

  int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof udph;

  Size -= header_size;
  const u_char *data = Buffer + header_size;
  int i = 0;
  char buffer[BUFSIZ];
  while (data[i] != '\0') {
    buffer[i] = data[i];
    ++i;
  }
  buffer[i] = '\0';

  int j = 0;
  --i;
  while (j < i) {
    char temp = buffer[i];
    buffer[i] = buffer[j];
    buffer[j] = temp;
    --i; ++j;
  }

  cout << "Reverse String - " << buffer << endl;

  strcpy(result, buffer);
}