
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ipc.h>
#include <linux/ip.h>
#include <linux/udp.h>

#define MAXLINE 4096
#define BUFFSIZE 8192
#define LISTENQ 1024
#define SERV_PORT 9877

int recv_fd(int socket);
int send_fd(int socket, int fd_to_send);

#define IP "127.0.0.1"

int main(int argc, char* argv[]){
	int udpfd;
	struct sockaddr_in udpip;
	int udpiplen;

	if((udpfd = socket(AF_INET, SOCK_STREAM, 2)) < 0){
		perror("Socket error!"); exit(0);
	}

	udpip.sin_family = AF_INET;
	udpip.sin_port = htons(atoi(argv[1]));
	udpip.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	udpiplen = sizeof(udpip);

	while(1){
		printf("Input message: ");
		char msg[2048];
		scanf("%s", msg);
		sendto(udpfd, msg, strlen(msg)+1, 0, (struct sockaddr*)&udpip, udpiplen);
	}
	close(udpfd);
	return 0;
}