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
#include <netinet/in.h>
#include <string.h>
#include<stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <sys/socket.h>
#include <sys/stat.h> 
#include <sys/select.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>

#define __USE_BSD	/* use bsd'ish ip header */
#include <sys/socket.h>	/* these headers are for a Linux system, but */
#include <netinet/in.h>	/* the names on other systems are easy to guess.. */
#include <netinet/ip.h>
#define __FAVOR_BSD	/* use bsd'ish tcp header */
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>

char *ADDRESS="./socket2";

unsigned short		/* this function generates header checksums */
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

int send_fd(int socket, int fd_to_send) {
    static struct cmsghdr *control_message = NULL;
    struct iovec iov[1];
    struct msghdr socket_message;
    
    char buf[2] = {0, 0};
    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    socket_message.msg_iov = iov;
    socket_message.msg_iovlen = 1;
    socket_message.msg_name = NULL;
    socket_message.msg_namelen = 0;
    
    control_message = malloc(CMSG_LEN(sizeof(int)));
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
    socket_message.msg_control = control_message;
    socket_message.msg_controllen = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(control_message) = fd_to_send;

    if(sendmsg(socket, &socket_message, 0) < 0) {
        perror("send failed");
        return -1;
    }

    return 0;
}

int recv_fd(int socket) {
    static struct cmsghdr *control_message = NULL;
    struct iovec iov[1];
    char buf[2];
    int newfd;

    struct msghdr socket_message;

    iov[0].iov_base = buf;
    iov[0].iov_len = sizeof(buf);
    socket_message.msg_iov = iov;
    socket_message.msg_iovlen = 1;
    socket_message.msg_name = NULL;
    socket_message.msg_namelen = 0;

    control_message = malloc(CMSG_LEN(sizeof(int)));
    socket_message.msg_control = control_message;
    socket_message.msg_controllen = CMSG_LEN(sizeof(int));

    if(recvmsg(socket, &socket_message, 0) < 0) {
        perror("recvmsg");
        return -1;
    }

    newfd = *(int *)CMSG_DATA(control_message);

    return newfd;
}


struct msg{
    long type ;
    char buf[2048];
};


int main(){
    struct sockaddr_un userv_addr;
	int usfd;

	if((usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(1);
	}
	memset(&userv_addr, 0, sizeof(userv_addr));
	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	if(connect(usfd, (struct sockaddr*)&userv_addr, sizeof(userv_addr)) == -1) {
		perror("connect error");
	}
    printf("Connected!\n");
	int rsfd= recv_fd(usfd);
    printf("Received the fd from s\n");
    char *docs = "doc1";

    char datagram[4096];	/* this buffer will contain ip header, tcp header,
			   and payload. we'll point an ip header structure
			   at its beginning, and a tcp header structure after
			   that to write the header values into it */
  
    struct ip *iph = (struct ip *) datagram;

    struct sockaddr_in sin;
			/* the sockaddr_in containing the dest. address is used
			   in sendto() to determine the datagrams path */

    sin.sin_family = AF_INET;
    sin.sin_port = htons (9013);/* you byte-order >1byte header values to network
                    byte order (not needed on big endian machines) */
    sin.sin_addr.s_addr = inet_addr ("127.0.0.1");

    memset(&datagram,0,4096);	/* zero out the buffer */

    /* we'll now fill in the ip/tcp header values, see above for explanations */
    iph->ip_hl = 5;
    iph->ip_v = 4;
    iph->ip_tos = 0;
    iph->ip_len = sizeof (struct ip);	/* no payload */
    iph->ip_id = htonl (54321);	/* the value doesn't matter here */
    iph->ip_off = 0;
    iph->ip_ttl = 255;
    iph->ip_p = 6;
    iph->ip_sum = 0;		/* set it to 0 before computing the actual checksum later */
    iph->ip_src.s_addr = inet_addr ("1.2.3.4");/* SYN's can be blindly spoofed */
    iph->ip_dst.s_addr = sin.sin_addr.s_addr;

    iph->ip_sum = csum ((unsigned short *) datagram, iph->ip_len >> 1);

    int one = 1;
    const int *val = &one;
    if (setsockopt (rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    printf ("\n\t Warning: I was not able to set HDRINCL!\n");


    if (sendto (rsfd, datagram, iph->ip_len, 0, (struct sockaddr *) &sin, sizeof (sin)) > 0)
        printf ("Success\n");
    
    printf("Sent fd to s\n");
    send_fd(usfd, rsfd);

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    

    while(1){
        int fd = recv_fd(usfd);

        int verify = rand()%2;

        struct msg msq;
        msq.type=2; //the type of receiver and sender should match for success
        if(verify == 0){
            strcpy(msq.buf, "Do not Verify! -from B");
        }
        else{
            strcpy(msq.buf, "Verify! -from B");
        }
        printf("Message sending: %s\n", msq.buf);
        msgsnd(msqid,&msq,2048,0);
        send_fd(usfd, fd);
    }
}
