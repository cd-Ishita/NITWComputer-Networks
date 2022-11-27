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

char IP[2048][2048];
char MAC[2048][2048];

int my_index = 0;
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

    printf("IP/MAC Table\n\n");
    for(int i=0;i<my_index;i++){
        printf("IP Address %d: %s\n", i+1, IP[i]);
        printf("MAC Address %d: %s\n", i+1, MAC[i]);
        printf("--------------------------------\n\n");
    }

    printf("%d", my_index);
	return 0;
}



void my_callback(u_char *args,const struct pcap_pkthdr *header,const u_char *packet){
	
    const struct ether_header *ehdr;
	struct ip *iphdr;
	const struct udphdr *udp;
	
	ehdr=(struct ether_header *)packet;
	iphdr=(struct ip *)(packet+14);
	
	printf("IP Address: %s\n", inet_ntoa(iphdr->ip_src));
    strcpy(IP[my_index], inet_ntoa(iphdr->ip_src));

    printf("MAC Address: %s\n", ether_ntoa((const struct ether_addr*)ehdr->ether_shost));
    strcpy(MAC[my_index], ether_ntoa((const struct ether_addr*)ehdr->ether_shost));
    my_index++;
    
    printf("----------------------------------------------\n\n");

	return;
}
