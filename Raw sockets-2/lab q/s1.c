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

struct thread_args{
    int id;
    int nsfd;
};
#define MAX 4
int cnt=0;
int A[MAX];

// Thread function, every thread for every client uses this
void* func1(void * arg){
    struct thread_args* args = (struct thread_args*)arg;
    int id = args->id;
    int nsfd = args->nsfd;
    
    char buf[2048] = {0};
    read(nsfd, buf, 2048);
    printf("File %d has sent %s\n", id, buf);
    cnt--;
    A[id] = 0;
    pthread_exit(NULL);
    
}
struct msg{
    long type ;
    char buf[2048];
};
int sfd;
int port_num;
struct sockaddr_in address;

void handler_func(int sig){
    printf("In signal handler!\n");
    int nsfd;
	int addrlen = sizeof(address);
    if((nsfd = accept(sfd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Accepted!\n");
    char buf[2048];
    read(nsfd, buf, 2048);
    printf("%s", buf);
}

int main(){

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);

    struct msg msq;
    msq.type=2; //the type of receiver and sender should match for success
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);

    signal(SIGUSR1, handler_func);

    

    port_num = 6060;
    
    int opt = 1;
    int addrlen = sizeof(address);
    
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
	
    sleep(15);
}