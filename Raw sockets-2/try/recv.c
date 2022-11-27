
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

#define IP "127.0.0.1"
int main(int argc, char const *argv[]){
	
	int sfd = socket(AF_INET,SOCK_RAW,2);
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	if(bind(sfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("Could not bind");exit(0);
	}
	char buffer[100];
	while(1)
	{
		memset(buffer,0,100);
		recvfrom(sfd,buffer,100,0,NULL,NULL);
		struct iphdr* ip;
		ip = (struct iphdr*)buffer;
		printf("Received: %s\n",buffer+(ip->ihl*4));
	}
	return 0;
}