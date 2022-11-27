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

char *ADDRESS="./mysocket";
int fifo2;
int score[3];
int sleep1 = 1;

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

void *ask(void *arg){
    
    int *id = (int *)arg;
    printf("Enter ques: ");
    char buf[2048] = "This is judge's question!";
    write(fifo2, buf, 2048);
    score[*id] = rand()%10;
    pthread_exit(NULL);
}

void *score_func(void *arg){
    int *id = (int *)arg;
    score[*id] = rand()%10;
    pthread_exit(NULL);
}

void performance_done(int sig){
    printf("got sig");
    sleep1 = 0;
}
struct msg{
    long type ;
    char buf[2048];
};
int connection(){
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
    return usfd;
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

int main(){
    signal(SIGUSR1, performance_done);

    int c;
    //printf("Enter char to send pid of Anchor to Screen: ");
    //scanf("%d", c);
    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
    struct msg msq;
    msq.type=2; //the type of receiver and sender should match for success
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);
	
    printf("Enter char to connect Anchor to Screen: \n");
    c = getchar();

    int usfd = connection();
    
    printf("Enter char to receive fd from Screen: \n");
    c = getchar();
    
    int fifo2 = recv_fd(usfd);
    
    pthread_t judge[3];
    
    printf("Enter char to wait for signal from Screen: \n");
    c = getchar();
    
    while(sleep1 != 0){
        sleep(2);
    }

    printf("Enter char to start scoring of Performer: \n");
    c = getchar();
    
    int rand1 = rand()%3;
    for(int i=0;i<3;i++){
        if(i == rand1){
            pthread_create(&judge[i], NULL, ask, (void *)i);
        }
        else{
            pthread_create(&judge[i], NULL, score_func, (void *)i);
        }
    }

    for(int i=0;i<3;i++){
        pthread_join(judge[i], NULL);
    }

    int total_score = score[0] + score[1] + score[2];
    printf("total score: %d", total_score);
    
    printf("Enter char to send score to Performer: ");
    c = getchar();
    
    char buf1[2048];
    sprintf(buf1, "%d", total_score);
    write_thread(fifo2, buf1);
}