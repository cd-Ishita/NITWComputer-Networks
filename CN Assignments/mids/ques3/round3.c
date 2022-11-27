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

char *ADDRESS="socket3";

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

int main(int argc, char *argv[]) {
	struct sockaddr_un userv_addr;
	char buf[100];
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
	printf("Connected!");

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
    
    while(1){
        int fd = recv_fd(usfd);
        printf("Received a client\n");
        int score = rand()%100;

        read(fd, buf, 2048);

        printf("Score of client %s: %d", buf, score);
        struct msg msq;
        sprintf(msq.buf, "%d", getpid());
        msq.type = atoi(buf);
        msgsnd(msqid, &msq, 2048, 0);
        send_fd(usfd, fd);
    }
    
    msgctl(msqid, IPC_RMID, NULL) ;
}