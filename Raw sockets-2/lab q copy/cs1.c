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
#include<pthread.h>
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

struct msg{
    long type ;
    char buf[2048];
};

int main(){

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
		struct msg msq;

    //P1 gets executed, it receives message and finds p2pid from message and sends SIGUSR1 to P2
    msgrcv(msqid,&msq,2048,2,0);
    printf("Message received! - %s", msq.buf);
    int pid[3];
    pid[0] = atoi(msq.buf);

    msgrcv(msqid,&msq,2048,3,0);
    printf("Message received! - %s", msq.buf);
    pid[1] = atoi(msq.buf);

    msgrcv(msqid,&msq,2048,4,0);
    printf("Message received! - %s", msq.buf);
    pid[2] = atoi(msq.buf);

    int port[3];
    port[0] = 6060;
    port[1] = 7070;
    port[2] = 8080;

    int rsfd[3];
    struct sockaddr_in addr[3];
    memset(&addr, 0, sizeof(addr));
    for(int i=0;i<3;i++){
        if ((rsfd[i] = socket(AF_INET, SOCK_RAW, 2)) < 0)
        {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
        addr[i].sin_family = AF_INET;
        addr[i].sin_port = htons(port[i]);
        addr[i].sin_addr.s_addr = inet_addr("127.0.0.1");
        int len = sizeof(addr[i]);
    }
    

    struct pollfd fds[3];

    memset(fds, 0 , sizeof(fds));

    for(int i=0;i<3;i++){
        fds[i].fd = rsfd[i];
        fds[i].events |= POLLIN;
    }
    int timeout = (3 * 60 * 1000);
    int connections = 0;
    while(connections != 2){
        int rc = poll(fds, 3, timeout);
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        for (int i=0;i<3;i++){
            if(fds[i].revents == 0)
                continue;

            if(fds[i].revents & POLLIN){
                connections++;
                unsigned char buffer[2048];
                int len = sizeof(addr[i]);
                int packetSize = recvfrom(rsfd[i], (char *)buffer, 4086,0, (struct sockaddr *)&addr[i],(socklen_t *)&len);

                kill(pid[i], SIGUSR1);

                fds[i].revents = 0;
            }
        }
    }

}