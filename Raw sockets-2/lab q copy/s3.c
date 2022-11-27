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

struct msg{
    long type ;
    char buf[2048];
};
int rsfd;
struct sockaddr_in addr;
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


void handler_func(int sig){
    printf("In handler function!\n");
    unsigned char buffer[2048];
    int len = sizeof(addr);
    int packetSize = recvfrom(rsfd, (char *)buffer, 4086,0, (struct sockaddr *)&addr,(socklen_t *)&len);

    unsigned char tempBuff[4086];
    for (int i = 0; i < 4086; i++)
        tempBuff[i] = buffer[i];
    struct iphdr *iph = (struct iphdr *)buffer;
    print_ipheader(iph);

    unsigned short ip_head_len = iph->ihl * 4;
    struct tcphdr *tcp_head = (struct tcphdr *)(tempBuff + ip_head_len);
    print_header(tempBuff + ip_head_len + tcp_head->th_off * 4, (packetSize - tcp_head->th_off * 4 - iph->ihl * 4));


    char str[2048] = "Server 1!!" ;
    sendto(rsfd, str,sizeof(str), 0, (const struct sockaddr *)&addr, sizeof(addr));
}

int main(){

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);

    struct msg msq;
    msq.type=4; //the type of receiver and sender should match for success
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);

    signal(SIGUSR1, handler_func);

    
    
    memset(&addr, 0, sizeof(addr));
    if ((rsfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    while(1){
        sleep(15);
    }
    
}