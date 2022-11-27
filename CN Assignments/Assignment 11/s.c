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

struct msg{
    long type;
    char buf[2048];
};

char *ADDRESS[3] = {"./socket1", "./socket2", "./socket3"};

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

#define __USE_BSD	/* use bsd'ish ip header */
#include <sys/socket.h>	/* these headers are for a Linux system, but */
#include <netinet/in.h>	/* the names on other systems are easy to guess.. */
#include <netinet/ip.h>
#define __FAVOR_BSD	/* use bsd'ish tcp header */
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>

#define P 9013		

unsigned short checksum (unsigned short *buf, int nwords){
    unsigned long sum;
    for (sum = 0; nwords > 0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    return ~sum;
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
int verify = 2;
void verified_func(int sig){
	verify = 1;
}

void not_verified_func(int sig){
    verify = 0;
}

int main(){
    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
		
    

    // 3 USFD
    struct sockaddr_un userv_addr[3], ucli_addr[3];
    int userv_len[3], ucli_len[3];
	int usfd[3], nufd[3];

    for(int i=0;i<3;i++){

        if ((usfd[i] = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket error");
        }else printf("Socket created\n");

        memset(&userv_addr[i], 0, sizeof(userv_addr[i]));
        userv_addr[i].sun_family = AF_UNIX;
        strcpy(userv_addr[i].sun_path, ADDRESS[i]);
        unlink(ADDRESS[i]);
        
        userv_len[i] = sizeof(userv_addr[i]) ;
        if (bind(usfd[i], (struct sockaddr*)&userv_addr[i], userv_len[i]) == -1) {
            perror("bind error");
        }else printf("Binded\n");

        if (listen(usfd[i], 5) == -1) {
            perror("listen error");
            exit(1);
        }else printf("Listening\n");

        ucli_len[i] = sizeof(ucli_addr[i]) ;
        if((nufd[i] = accept(usfd[i], (struct sockaddr*)&ucli_addr[i], &ucli_len[i])) < 0) {
            perror("accept");
            exit(1);
        }
        printf("Accepted\n");
    }


    // CREATE RAW SOCKET
    int rsfd = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);	/* open raw socket */

    printf("Sending raw socket to A\n");
    send_fd(nufd[0], rsfd);
    rsfd = recv_fd(nufd[0]);

    printf("Sending raw socket to B\n");
    send_fd(nufd[1], rsfd);
    rsfd = recv_fd(nufd[1]);

    printf("Sending raw socket to C\n");
    send_fd(nufd[2], rsfd);
    rsfd = recv_fd(nufd[2]);

    
    // CREATE WELL KNOWN TCP SOCKET
    int sfd, nsfd;
    int port_num;
    struct sockaddr_in address;

    port_num = 6060;
    
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))){
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

    // POLLING
    struct pollfd fds[1];
    memset(fds, 0 , sizeof(fds));

    fds[0].fd = sfd;
    fds[0].events |= POLLIN;

    int timeout = (3 * 60 * 1000);
    int connections = 0;
    printf("Start polling!\n");
    while(connections != 5){
        int rc = poll(fds, 1, timeout);
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        if(fds[0].revents == 0)
            continue;

        if(fds[0].revents & POLLIN){
            connections++;
            
            //TCP connections accept
            
            int addrlen = sizeof(address);
            if((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("Sending Client to A!\n");
            send_fd(nufd[0], nsfd);

            struct msg msq;

            msgrcv(msqid,&msq,2048,1,0);
            printf("Message content: %s\n", msq.buf);

            nsfd = recv_fd(nufd[0]);
	    write(nsfd, msq.buf, 2048);

            printf("Sending Client to B!\n");
            send_fd(nufd[1], nsfd);

            msgrcv(msqid,&msq,2048,2,0);
            printf("Message content: %s\n", msq.buf);
            
            nsfd = recv_fd(nufd[1]);
            write(nsfd, msq.buf, 2048);

            printf("Sending Client to C!\n");
            send_fd(nufd[2], nsfd);

            msgrcv(msqid,&msq,2048,3,0);
            printf("Message content: %s\n", msq.buf);

            nsfd = recv_fd(nufd[2]);
            write(nsfd, msq.buf, 2048);
            printf("Client verification done \n\n----------------------\n");
        }
    }
}
