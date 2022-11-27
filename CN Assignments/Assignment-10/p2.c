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
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>

char *ADDRESS="./platform2";
int end;
void input_finish(int sig, siginfo_t *info, void *context){
    end = 1;
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

int main(int argc, char *argv[]) {
    printf("-------Welcome to Platform 2!-------\n");
	int fd = open("pid.txt", O_RDONLY);
    char buff[100];
    read(fd, buff, 100);
    int parent_pid = 0;
    int i = 0;
    while(buff[i] != '\0'){
        parent_pid = parent_pid*10 + (int)buff[i] - 48;
        i++;
    }
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

    printf("Platform 2 is connected and online!\n");
    // Use SIGUSR2 to find that the platform is empty
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = input_finish;
    sigaction(SIGUSR2, &sa, NULL);
    kill(parent_pid, SIGUSR1);

	while(1){
        int nsfd= recv_fd(usfd) ;

        printf("I have received an fd\n");
    
        char pid[2048];
        int p = getpid();
        sprintf(pid, "%d", p);
        send(nsfd, pid, 2048, 0);
        char buffer[2048];
        while(end != 1){
            read(nsfd, buffer, 1024);
            printf("%s\n", buffer);
            sleep(1);
        }
        printf("Train has left the station\n");
        kill(parent_pid, SIGUSR2);
        printf("I have signalled Station Master that I am empty\n");
    }
    //char buff[100];
    //read(usfd, buff, 100);
    //printf("%s\n", buff);
	return 0;
}
