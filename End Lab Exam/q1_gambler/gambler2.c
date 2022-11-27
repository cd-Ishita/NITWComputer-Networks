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

int sfd;
int port_num;
struct sockaddr_in address;

void* func1(void* args){
    int addrlen = sizeof(address);
    int nsfd;
    while(1){
        if((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted\n");
        char buffer[1024] = {0};
        read(nsfd, buffer, 1024);
        //write(nsfd, buffer, 2048);
        printf("Message from client %s\n", buffer);

        int flag = 0;
        for(int i=0;i<my_index;i++){
            if(strcmp(IP[i], buffer) == 0){
                write(nsfd, MAC[i], 1024);
                flag = 1;
                break;
            }
        }
        if(flag == 0){
            printf("IP/MAC information doesnt exist for %s\n", buffer);
        }
    }
}

int main(int argc,char *arv[]){

    

    port_num = 6060;
    
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_num);
    
    // Forcefully attaching socket to the port 8080 
    if (bind(sfd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sfd, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("sfd is listening for connections\n");
	
    pthread_t thread;
    pthread_create(&thread, NULL, func1, NULL);
    
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

    while(1){

    }
}



void my_callback(u_char *args,const struct pcap_pkthdr *header,const u_char *packet){
	
    const struct ether_header *ehdr;
	struct ip *iphdr;
	const struct udphdr *udp;
	
	ehdr=(struct ether_header *)packet;
	iphdr=(struct ip *)(packet+14);
	
	//printf("IP Address: %s\n", inet_ntoa(iphdr->ip_src));
    strcpy(IP[my_index], inet_ntoa(iphdr->ip_src));

    //printf("MAC Address: %s\n", ether_ntoa((const struct ether_addr*)ehdr->ether_shost));
    strcpy(MAC[my_index], ether_ntoa((const struct ether_addr*)ehdr->ether_shost));
    my_index++;
    
    //printf("----------------------------------------------\n\n");

	return;
}
