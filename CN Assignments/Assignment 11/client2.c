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


#define __USE_BSD	/* use bsd'ish ip header */
#include <sys/socket.h>	
#include <netinet/in.h>	
#include <netinet/ip.h>
#define __FAVOR_BSD	/* use bsd'ish tcp header */
#include <netinet/tcp.h>
#include <unistd.h>

#define P 9013	



#define PORT 6060
int main(){
    int s = socket (AF_INET, SOCK_RAW, 0);	


    int n = 0 ;
  				
    int one = 1;
    const int *val = &one;
    if (setsockopt (s, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
        printf ("Warning: Cannot set HDRINCL!\n");

    char datagram[4096];	

    struct ip *iph = (struct ip *) datagram;

    struct tcphdr *tcph = (struct tcphdr *) datagram + sizeof (struct ip);

    struct sockaddr_in sin,sin2; socklen_t len = sizeof(sin2) ;

    memset(&sin,0,sizeof(sin)) ;

    sin.sin_family = AF_INET;
    sin.sin_port = htons (P);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(s,(struct sockaddr *) &sin,sizeof(sin)) ;
 
   
    if( (n = recvfrom(s,datagram,4096,0,(struct sockaddr *) &sin,&len) ) < 0)	
	    printf ("error\n");
      
	datagram[n] = 0 ; printf("\n\nHeaders have ben received as : ");
    printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",iph->ip_hl,iph->ip_v ,  iph->ip_tos,  iph->ip_len,  iph->ip_id ,  iph->ip_off,  iph->ip_ttl,  iph->ip_p,  iph->ip_sum,  iph->ip_src.s_addr,  iph->ip_dst.s_addr,  tcph->th_sport,  tcph->th_dport,  tcph->th_seq,  tcph->th_ack,  tcph->th_x2 ,  tcph->th_off,  tcph->th_flags,  tcph->th_win,  tcph->th_sum,  tcph->th_urp) ;
    printf("\n");
    printf("\n");
    
    int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "S1";
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}
    printf("Client has connected\n");
	
	char buffer[2048];
    read(sock, buffer, 2048);
	printf("Received this from server: %s\n", buffer);
	
    read(sock, buffer, 2048);
	printf("Received this from server: %s\n", buffer);
	
    read(sock, buffer, 2048);
	printf("Received this from server: %s\n", buffer);
	
    close(sock);
	return 0;
}