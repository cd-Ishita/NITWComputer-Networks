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
void *read(void *arg)
{
	int sfd=socket(AF_INET,SOCK_STREAM,NULL);
	struct sockaddr_in addr;
	int addrlen=sizeof(addr);
	addr.sin_family = AF_INET; 
    	addr.sin_addr.s_addr = INADDR_ANY; 
    	addr.sin_port = htons( 8080 );
	bind(sfd,(struct sockaddr *)&addr,sizeof(addr));
	listen(sfd,3);
	int nsfd=accept(sfd,(struct sockaddr *)&addr,(socklen_t*)&(addrlen));
	char buff[1024];
	while(1){
	recv(nsfd,buff,1024,0);
	cout<<buff<<endl;}
}
void *read1(void *arg)
{
	int sfd=socket(AF_INET,SOCK_STREAM,NULL);
	struct sockaddr_in addr;
	int addrlen=sizeof(addr);
	addr.sin_family = AF_INET; 
    	addr.sin_addr.s_addr = INADDR_ANY; 
    	addr.sin_port = htons( 8081 );
	bind(sfd,(struct sockaddr *)&addr,sizeof(addr));
	listen(sfd,3);
	int nsfd=accept(sfd,(struct sockaddr *)&addr,(socklen_t*)&(addrlen));
	char buff[1024];
	while(1){
	recv(nsfd,buff,1024,0);
	cout<<buff<<endl;}
}
int main()
{
	 pthread_t t1,t2;
	 pthread_create(&t1,NULL,read,NULL);
	 pthread_create(&t2,NULL,read1,NULL);
	 pthread_join(t1,NULL);
	 pthread_join(t2,NULL);
}
