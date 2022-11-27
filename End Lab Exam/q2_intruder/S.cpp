#include<iostream>
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

void setup_sfd(int &sfd, int port, char* ip_addr)
{
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ip_addr);
  address.sin_port = htons(port);

  if (bind(sfd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]) {
    int sfd = 0;

    if (argc < 3) {
      cout << "Usage = PROGRAM PORT IP_ADDR\n";
      exit(EXIT_FAILURE);
    }
    setup_sfd(sfd, atoi(argv[1]), argv[2]);

    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    while(1){

        char buffer[BUFFER_SIZE];

        recvfrom(sfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&client_addr, &addrlen);

        cout << "Received from Client - " << buffer << endl;

        int i = 0;
        while (buffer[i] != '\0') {
          buffer[i] = toupper(buffer[i]);
          ++i;
        }

        cout << "Transformed Data - " << buffer << endl;

        sendto(sfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&client_addr, addrlen);
    }
}