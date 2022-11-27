#include <iostream>
#include <pthread.h>
#include<arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include<sys/socket.h>
using namespace std;
int main()
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(8081);
	connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	char buff[1024];
	while(1){
	cout<<"Enter msg ";
	cin>>buff;
	send(sock,buff,1024,0);
	cout<<endl;
}
}
