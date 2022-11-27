#include<stdio.h>
#include<iostream>
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
#include<pcap.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
using namespace std;

#define BUFFER_SIZE BUFSIZ
int sfd = 0, rsfd = 0;

void connect_sfd(int &sfd, int port, char *ip_addr){

    if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        printf("\n Socket creation error \n");
        return;
    }
}

void *thread_service(void *args){
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	while (1){
		char buffer[BUFSIZ];
		recvfrom(rsfd, buffer, BUFSIZ, 0, (sockaddr *)&addr, &addrlen);
		struct iphdr *ip;
		ip = (struct iphdr *)buffer;
		cout << "On rsfd, got data - " << (buffer + (ip->ihl) * 4) << endl;
  	}
}

int main(int argc, char *argv[]){

	if ((rsfd = socket(AF_INET, SOCK_RAW, 201)) < 0)
		perror("rsfd");
	int optval = 1;
	if (setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int)) < 0)
		perror("setsockopt");

	if (argc < 3)
	{
		cout << "Usage = PROGRAM PORT IP_ADDR\n";
		exit(EXIT_FAILURE);
	}

	pthread_t t1;

	pthread_create(&t1, NULL, thread_service, NULL);

	connect_sfd(sfd, atoi(argv[1]), argv[2]);

	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	address.sin_family = AF_INET;
	address.sin_port = htons(atoi(argv[1]));
	address.sin_addr.s_addr = inet_addr(argv[2]);

	
  	for (;;){
	  	char buffer[BUFFER_SIZE];
		cout << "Enter data to capitalize - ";
		cin >> buffer;
		sendto(sfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&address, addrlen);

		int sz = recvfrom(sfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&address, &addrlen);
		buffer[sz] = '\0';

		cout << "Received from server - " << buffer << endl;
		//sz = recvfrom(sfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&address, &addrlen);
		//buffer[sz] = '\0';
  	}
}