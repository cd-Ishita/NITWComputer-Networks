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

#define MAX 3
int cnt=0;
int A[MAX];

char *ADDRESS[3]= {"./platform1", "./platform2", "./platform3"};
int platform_pid[3];

void get_pid(int sig, siginfo_t *info, void *context){
    int i=0;
    printf("pid");
    while(platform_pid[i] != 0){
        i++;
    }
    platform_pid[i] = info->si_pid;
}
int empty[3]; // to check which platforms are empty right now
void platform_empty(int sig, siginfo_t *info, void *context){
    int i=0;
    while(platform_pid[i] != info->si_pid){
        i++;
    }
    empty[i] = 1;
    printf("Platform %d is now empty\n", i+1);
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
int main(int argc, char *argv[]) {
    printf("----------- Welcome to Station Master ---------\n");

    int fd = open("pid.txt", O_WRONLY);
    char pid[2048];
    sprintf(pid, "%d", getpid());
    write(fd, pid, 2048);
    close(fd);

    int num = 3;
	struct sockaddr_un userv_addr[num],ucli_addr[num];
    int userv_len[num],ucli_len[num];
	char buf[100];
	int usfd[num],nufd[num];

    for(int i=0;i<num;i++){
        if ( (usfd[i] = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket error");
        }

        memset(&userv_addr[i], 0, sizeof(userv_addr[i]));
        userv_addr[i].sun_family = AF_UNIX;
        strcpy(userv_addr[i].sun_path, ADDRESS[i]);
        unlink(ADDRESS[i]);
        
        userv_len[i]=sizeof(userv_addr[i]) ;
        if (bind(usfd[i], (struct sockaddr*)&userv_addr[i], userv_len[i]) == -1) {
            perror("bind error");
        }

        if (listen(usfd[i], 5) == -1) {
            perror("listen error");
            exit(1);
        }

        ucli_len[i]=sizeof(ucli_addr[i]) ;
        if((nufd[i] = accept(usfd[i], (struct sockaddr*)&ucli_addr[i], &ucli_len[i])) < 0) {
            perror("accept");
            exit(1);
        }
        printf("usfd %d is accepted and connected\n", i+1);
    }
	

    int sfd[num];
    int port_num[num];
    struct sockaddr_in address[num];

    //configure to take this info from config file
    port_num[0] = 6060;
    port_num[1] = 7070;
    port_num[2] = 9090;

    for(int i=0;i<num;i++){
        int opt = 1;
        int addrlen = sizeof(address[i]);
        
        // Creating socket file descriptor
        if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port 8080
        if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address[i].sin_family = AF_INET;
        address[i].sin_addr.s_addr = INADDR_ANY;
        address[i].sin_port = htons(port_num[i]);
        
        // Forcefully attaching socket to the port 8080
        if (bind(sfd[i], (struct sockaddr *)&address[i], sizeof(address[i]))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(sfd[i], 5) < 0){
            perror("listen");
            exit(EXIT_FAILURE);
        }
        printf("sfd%d is listening for connections\n", i+1);
    }

    // --------- signal handling from platforms --------//
    // Use SIGUSR1 to find pid of all platforms
    
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);

    // Use SIGUSR2 to find that the platform is empty
    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = platform_empty;
    sigaction(SIGUSR2, &sa1, NULL);
    // -------------------- select ----------------
    
    memset(empty, 1, sizeof(num*empty[0]));
    for(int i=0;i<num;i++){
        empty[i] = 1;
    }

    //pthread_t tid[num];
    int cnt = 0;
    fd_set rfds;
    int addrlen = sizeof(address); 
    
    memset(A, 0, num*sizeof(A[0]));

    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);

		int ma=-1;
		for(int i=0;i<num;i++){
			FD_SET(sfd[i], &rfds);
            if(ma<sfd[i]){
                ma = sfd[i];
            }
            if(A[i] == 1){
                cnt++;
            }
	    }
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		
        if(count>0){
			int free=0;
			for(int i=0;i<num;i++){
				char ch[120];
				if(cnt < num){	
                    // Check is sfd[i] is a part of rfds
                    if(FD_ISSET(sfd[i],&rfds)){	
						int nsfd;
   						if ((nsfd = accept(sfd[i], (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) { 
                            perror("accept"); 
                            exit(EXIT_FAILURE); 
    					} 
    					else{
                            printf("I have received request from train\n");
                            int it = 0;
                            while(it < num && empty[it] != 1){
                                it++;
                            }
                            if(it == num){
                                printf("No platform is free, train is sent away!\n");
                            }
                            else{
                                empty[it] = 0;
                                send_fd(nufd[it], nsfd);
                                printf("Sent train to Platform %d\n", it+1);     
                            }
    					}
					}
					else{
                        free++;
                    }
				}
				else{
					printf("Platforms are full, sleeping now\n");
					sleep(1);
				}	
			}
			if(free == num){
                // It comes to this if none of the sockets received a connection
			    printf("Timeout reached\n");
            }
		}
	}
	
	return 0;
}