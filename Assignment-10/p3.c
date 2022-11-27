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

int end = 0;

void input_finish(int sig, siginfo_t *info, void *context){
    end = 1;
}

int recv_fd(int socket){
    int sent_fd, available_ancillary_element_buffer_space;
    struct msghdr socket_message;
    struct iovec io_vector[1];
    struct cmsghdr *control_message = NULL;
    char message_buffer[1];
    char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

    memset(&socket_message, 0, sizeof(struct msghdr));
    memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

    /* setup a place to fill in message contents */ io_vector[0].iov_base = message_buffer; io_vector[0].iov_len = 1; socket_message.msg_iov = io_vector; socket_message.msg_iovlen = 1;
    /* provide space for the ancillary data */ socket_message.msg_control = ancillary_element_buffer; socket_message.msg_controllen = CMSG_SPACE(sizeof(int));
    if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0) return -1;
        if(message_buffer[0] != 'F'){/* this did not originate from the above function */ 
            return -1;
        }
    
    if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC){
        /* we did not provide enough space for the ancillary element array */  
        return -1; 
    }

    /* iterate ancillary elements */
    for(control_message = CMSG_FIRSTHDR(&socket_message);
    
    control_message != NULL;
    control_message = CMSG_NXTHDR(&socket_message, control_message)){
        if( (control_message->cmsg_level == SOL_SOCKET) && (control_message->cmsg_type == SCM_RIGHTS) ) {
            sent_fd = *((int *) CMSG_DATA(control_message));
            return sent_fd;
        }
    }
    return -1;
}

int main(){
    //kill(getppid(), SIGUSR1);

    // Use SIGUSR2 to find that the platform is empty
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = input_finish;
    sigaction(SIGUSR2, &sa, NULL);

    int usfd;
    struct sockaddr_un userv_addr;
    int userv_len, ucli_len;
    printf("1");
    usfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(usfd == -1){
        perror("\nsocket");
        return(1);
    }
    printf("1");
    bzero(&userv_addr, sizeof(userv_addr));
    userv_addr.sun_family = AF_UNIX;
    strcpy(userv_addr.sun_path, "platform3");
    printf("1");
    userv_len = sizeof(userv_addr);
    printf("1");
    if(connect(usfd, (struct sockaddr *)&userv_addr, userv_len) == -1){
        perror("\n connect");
        return (1);
    }
    else{
        printf("\nconnect successful");
    }

    int nsfd = recv_fd(usfd);

    char buffer[2048];
    while(end != 1){
        read(nsfd, buffer, 1024);
        printf("%s", buffer);
        printf("\n");
    }

    //kill(getppid(), SIGUSR2);
}