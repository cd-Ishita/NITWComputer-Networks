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
struct msg{
    long type ;
    char buf[2048];
};
char *ADDRESS="./mysocket";
int pid_A;


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

char* read_thread(int fd){
    char buf[2048];
    read(fd, buf, 2048);
    char* buf1;
    strcpy(buf1, buf);
    return buf1;
}

void write_thread(int fd, char buf[2048]){
    write(fd, buf, 2048);
}

int connection(){
    struct sockaddr_un userv_addr ,ucli_addr;
    int userv_len,ucli_len;

	int usfd,nufd;

	if ( (usfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
	}else printf("Socket created\n");

	memset(&userv_addr, 0, sizeof(userv_addr));
	userv_addr.sun_family = AF_UNIX;
	strcpy(userv_addr.sun_path, ADDRESS);
	unlink(ADDRESS);
	
    userv_len=sizeof(userv_addr) ;
	if (bind(usfd, (struct sockaddr*)&userv_addr, userv_len) == -1) {
		perror("bind error");
	}else printf("Binded\n");

	if (listen(usfd, 5) == -1) {
		perror("listen error");
		exit(1);
	}else printf("Listening\n");

    ucli_len=sizeof(ucli_addr) ;
	if((nufd = accept(usfd, (struct sockaddr*)&ucli_addr, &ucli_len )) < 0) {
	    perror("accept");
	    exit(1);
	}
	printf("Accepted\n");
    return nufd;
}

int main(){
    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    struct msg msq;

    //P1 gets executed, it receives message and finds p2pid from message and sends SIGUSR1 to P2
    msgrcv(msqid,&msq,2048,2,0);
    pid_A = atoi(msq.buf);
    
    int c;

    printf("Enter char for Screen to connect to performer: \n");
    c = getchar();

    mkfifo("FIFO1", 0666);
    mkfifo("FIFO2", 0666);

    int fifo1 = open("FIFO1", O_RDONLY);
    int fifo2 = open("FIFO2", O_WRONLY);

    printf("Enter char for Screen to show performance: \n");
    c = getchar();

    //while(1){
        printf("1");
        char* buf = read_thread(fifo1);
        
        printf("Performer sent: %s\n", buf);

        printf("Enter char for Screen to show performance: \n");
        c = getchar();
        buf = read_thread(fifo1);
        if(strcmp(buf, "/d") == 0){
            //break;
        }
    //}
    printf("Enter char to send signal to A: \n");
    c = getchar();

    kill(SIGUSR1, pid_A);
    
    printf("Enter char for Screen to connect to Anchor: \n");
    c = getchar();

    int nufd = connection();

    printf("Enter char for Screen to send fd to Anchor: \n");
    c = getchar();
    
    send_fd(nufd, fifo2);

    printf("Enter char for Screen to show answer of performer: \n");
    c = getchar();
    
    char *buf1 = read_thread(fifo1);
    printf("Performer answers: %s\n", buf1);
}