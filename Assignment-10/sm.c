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
#include <sys/un.h>

struct thread_args{
    int id;
    int nsfd;
};
#define MAX 4
int cnt=0;
int A[MAX];

char *address[3] = {"platform1", "platform2", "platform3"};
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
}

int send_fd(int socket, int fd_to_send){
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];

    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
    int available_ancillary_element_buffer_space;

    message_buffer[0] = 'F';
    io_vector[0].iov_base = message_buffer;
    io_vector[0].iov_len = 1;

    memset(&socket_message, 0, sizeof(struct msghdr));
    socket_message.msg_iov = io_vector;
    socket_message.msg_iovlen = 1;

    available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
    memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
    socket_message.msg_control = ancillary_element_buffer;
    socket_message.msg_controllen = available_ancillary_element_buffer_space;

    control_message = CMSG_FIRSTHDR(&socket_message);
    control_message->cmsg_level = SOL_SOCKET;
    control_message->cmsg_type = SCM_RIGHTS;
    control_message->cmsg_len = CMSG_LEN(sizeof(int));
    *((int *) CMSG_DATA(control_message)) = fd_to_send;

    return sendmsg(socket, &socket_message, 0);
}

int main(){
    // --------- platform processes ------------ //
    /*
    int c = fork();
    if(c == 0){
        char *args[] = {"./p1", NULL};
        execv(args[0], args);
    }
    platform_pid[0] = c;

    c = fork();
    if(c == 0){
        char *args[] = {"./p2", NULL};
        execv(args[0], args);
    }
    platform_pid[1] = c;

    c = fork();
    if(c == 0){
        char *args[] = {"./p3", NULL};
        execv(args[0], args);
    }
    platform_pid[2] = c;
    */
    int num = 3;
    
    // ---------- defining sockets for platforms
    int usfd[num];
    
    struct sockaddr_un userv_addr[num], ucli_addr[num];
    int userv_len[3],ucli_len[3];
    int nusfd[num];
    for(int i=0;i<num;i++){
        
        usfd[i] = socket(AF_UNIX , SOCK_STREAM , 0);
        if(usfd[i] < 0) {
            perror("socket failure\n");
        }

        bzero(&userv_addr[i],sizeof(userv_addr[i]));
        userv_addr[i].sun_family = AF_UNIX;
        strcpy(userv_addr[i].sun_path, address[i]);
        unlink(address[i]);

        userv_len[i] = sizeof(userv_addr[i]);
        if(bind(usfd[i], (struct sockaddr *)&userv_addr[i], userv_len[i])==-1) {
            perror("bind failure\n");
        }
        if(listen(usfd[i], 5) == -1) {
            perror("listen failure\n");
        }
        ucli_len[i]=sizeof(ucli_addr[i]);
        
        printf("usfd%d is listening for connections\n", i+1);

        ucli_len[i] = sizeof(ucli_addr[i]);
        nusfd[i] = accept(usfd[i], (struct sockaddr *)&ucli_addr[i], &ucli_len[i]);
        printf("connected\n");
    }

    // --------- defining sockets for trains --------//
    
    
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

    //pthread_t tid[num];
    int cnt = 0;
    fd_set rfds;
    int addrlen = sizeof(address); 
    
    memset(A, 0, num*sizeof(A[0]));

    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);
		FD_SET(sfd[0],&rfds);
		FD_SET(sfd[1],&rfds);
		FD_SET(sfd[2],&rfds);

		int ma=-1;
		for(int i=0;i<num;i++){
			if(ma<sfd[i])
	    		ma=sfd[i];
	    }
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
                            /*
    						printf("\ncount: %d\n", cnt);
                            struct thread_args arg;
                            arg.id = i;
                            arg.nsfd = nsfd;
                            pthread_create(&tid[i],NULL,func1,(void *)&arg);
    						
                            for(int it=0;it<num;it++){
                                if(A[it] == 0){
                                    A[it] = 1;
                                    break;
                                }
                            }*/

                            int it = 0;
                            while(it < num && empty[it] != 1){
                                it++;
                            }
                            if(it == num){
                                printf("No platform is free, train is sent away!\n");
                            }
                            else{
                                empty[it] = 0;
                                send_fd(usfd[it], nsfd);     
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
                    //for(int i=0;i<num;i++){
                        //pthread_join(tid[i], NULL);
                    //}
				}	
			}
			if(free == num){
                // It comes to this if none of the sockets received a connection
			    printf("Timeout reached\n");
            }
		}
	} 
}