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

char *address[3] = {"socket1", "socket2", "socket3"};

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

int stop = 0;

void handler_func(int sig){
    stop = 1;
}

int threshold_marks = 50;
sem_t *r1, *r2, *r3;

int R1 = 0, R2 = 0, R3 = 0;
int nufd[3], usfd[3];

struct thread_args{
    int fd;
    int id;
};

void *thread_func(void *args){
    struct thread_args* arg = (struct thread_args*)args;

    int fd = arg->fd;
    int id = arg->id;
    int marks = 70;
    int rounds[3];
    rounds[0] = 0;
    rounds[1] = 0;
    rounds[2] = 0;

    if(marks > threshold_marks){
        while(1){
            if(rounds[0] == 0){
                printf("Client %d is in round 1\n", id);
                sem_wait(r1);
            
                send_fd(nufd[0], fd);
                fd = recv_fd(nufd[0]);
                rounds[0] = 1;
                sem_post(r1);
            }
            else if(rounds[1] == 0){
                sem_wait(r2);
                printf("Client %d is in round 2\n", id);
            
                send_fd(nufd[1], fd);
                fd = recv_fd(nufd[1]);
                rounds[1] = 1;
                sem_post(r2);
            }
            else if(rounds[2] == 0){
                sem_wait(r3);
                printf("Client %d is in round 3\n", id);

                send_fd(nufd[2], fd);
                fd = recv_fd(nufd[2]);
                rounds[2] = 1;
                sem_post(r3);
            }
            else{
                break;
            }
        }
    }
}
int main(int argc, char *argv[]) {

    r1 = sem_open("r1", O_CREAT, 0666, 0);
    r2 = sem_open("r2", O_CREAT, 0666, 0);
    r3 = sem_open("r3", O_CREAT, 0666, 0);
	struct sockaddr_un userv_addr[3] ,ucli_addr[3];
    int userv_len[3],ucli_len[3];
	char buf[100];

    for(int i=0;i<3;i++){
        if ( (usfd[i] = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket error");
        }else printf("Socket created\n");

        memset(&userv_addr[i], 0, sizeof(userv_addr[i]));
        userv_addr[i].sun_family = AF_UNIX;
        strcpy(userv_addr[i].sun_path, address[i]);
        unlink(address[i]);
        
        userv_len[i]=sizeof(userv_addr[i]) ;
        if (bind(usfd[i], (struct sockaddr*)&userv_addr[i], userv_len[i]) == -1) {
            perror("bind error");
        }else printf("Binded\n");

        if (listen(usfd[i], 5) == -1) {
            perror("listen error");
            exit(1);
        }else printf("Listening\n");

        ucli_len[i]=sizeof(ucli_addr[i]) ;
        if((nufd[i] = accept(usfd[i], (struct sockaddr*)&ucli_addr[i], &ucli_len[i] )) < 0) {
            perror("accept");
            exit(1);
        }
        printf("Accepted\n");
    }

    int sfd, new_sfd;
    int port_num;
    struct sockaddr_in Address;

    port_num = 6060;
    
    int opt = 1;
    int addrlen = sizeof(Address);
    
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
    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port = htons(port_num);
    
    // Forcefully attaching socket to the port 8080
    if (bind(sfd, (struct sockaddr *)&Address, sizeof(Address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sfd, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("sfd is listening for connections\n");
		
    int i = 0;
    pthread_t threads[100];
    signal(SIGUSR1, handler_func);

    while(1){
        int addrlen = sizeof(Address);
        if((new_sfd = accept(sfd, (struct sockaddr *)&Address, (socklen_t*)&addrlen))<0){
            perror("accept");
            exit(EXIT_FAILURE);
        }
        struct thread_args args;
        args.fd = new_sfd;
        args.id = i;
        pthread_create(&threads[i], NULL, thread_func, &args);
        i++;

        if(stop == 1){
            break;
        }
    }
	
	return 0;
}