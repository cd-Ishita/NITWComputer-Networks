#include<stdio.h>
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
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include<arpa/inet.h>
#include<poll.h>

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
	// send to all receivers  
    int rsfd;
    struct sockaddr_in addr, cliaddr;
    int len = sizeof(cliaddr);
    if ((rsfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(6060);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(rsfd, (const struct sockaddr *)&addr,
         sizeof(addr));

    char *str = "Client1";
    sendto(rsfd, str,sizeof(str), 0, (const struct sockaddr *)&addr, sizeof(addr));

	unsigned char buffer[2048];
    len = sizeof(addr);
    int packetSize = recvfrom(rsfd, (char *)buffer, 4086,0, (struct sockaddr *)&addr,(socklen_t *)&len);

    unsigned char tempBuff[4086];
    for (int i = 0; i < 4086; i++)
        tempBuff[i] = buffer[i];
    struct iphdr *iph = (struct iphdr *)buffer;
    print_ipheader(iph);

    unsigned short ip_head_len = iph->ihl * 4;
    struct tcphdr *tcp_head = (struct tcphdr *)(tempBuff + ip_head_len);
    print_header(tempBuff + ip_head_len + tcp_head->th_off * 4, (packetSize - tcp_head->th_off * 4 - iph->ihl * 4));

	return 0;
}