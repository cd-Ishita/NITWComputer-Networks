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

int main(){

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
		struct msg msq;

    //P1 gets executed, it receives message and finds p2pid from message and sends SIGUSR1 to P2
    msgrcv(msqid,&msq,2048,2,0);
    printf("Message received! - %s", msq.buf);
    int pid = atoi(msq.buf);

    // Number of sockets
    int num = 1;
    int sfd;
    int port_num;
    struct sockaddr_in address;

    //configure to take this info from config file
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
    printf("sfd%d is listening for connections\n", 1);

    // -------------------- select ----------------

    pthread_t tid[num];
    int cnt = 0;
    fd_set rfds;
    addrlen = sizeof(address); 
    
    memset(A, 0, num*sizeof(A[0]));

    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);
		FD_SET(sfd,&rfds);

		int ma=-1;
        ma=sfd;
	    
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		//After the select call, rfds set has been modified to contain those sfd that are ready to read
		
        //Check if all A are being used rn
        // cnt suggests can we take another connection? or are all being used rn
        for(int it=0;it<num;it++){
            if(A[it] == 1){
                cnt++;
            }
        }

        if(count>0){
            if(FD_ISSET(sfd,&rfds)){
                printf("sending signal: pid %d", pid);
                kill(pid, SIGUSR1);
                break;
 			}
		}
	}
}