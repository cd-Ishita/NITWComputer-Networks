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
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include<bits/stdc++.h>
using namespace std;
#define MAXLINE 1024 
int usfd;
int cnt=0;
int A[3]={0};
int sfd[3];
pthread_t tid;
fd_set rfds;
struct pollfd pfds[3];
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address); 
struct sockaddr_in s_addr, c_addr; 
char buffer[MAXLINE]; 
char *hello = "Hello from server"; 
#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static
void send_fd(int socket, int *fds, int n)  // send fd by socket
{
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
        memset(buf, '\0', sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(n * sizeof(int));

        memcpy ((int *) CMSG_DATA(cmsg), fds, n * sizeof (int));

        if (sendmsg (socket, &msg, 0) < 0)
                handle_error ("Failed to send message");
}
void* func(void* arg)
{
	
	while(1)
	{
		FD_ZERO(&rfds);
		FD_SET(sfd[0],&rfds);
		FD_SET(sfd[1],&rfds);
		FD_SET(sfd[2],&rfds);
		int ma=-1;
		for(int i=0;i<3;i++)
		{
			if(ma<sfd[i])
	    		ma=sfd[i];
	    	}
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		if(count>0)
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(i==0||i==2)
				{	
					if(FD_ISSET(sfd[i],&rfds))
					{	
						int nsfd;
   						 if ((nsfd = accept(sfd[i], (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    						 { 
        						 perror("accept"); 
        						 exit(EXIT_FAILURE); 
    						 } 
    						 cout<<"hi"<<endl;
    						int fds[1]={nsfd};
    						send_fd(usfd,fds,1);
    						cout<<"sent"<<endl;
    						cout<<fds[0]<<endl;
						
					}
					
				}
				else if(i==1)
				{
					if(FD_ISSET(sfd[i],&rfds))
					{
						int n; 
						socklen_t len = sizeof(c_addr);
						n = recvfrom(sfd[i], (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &c_addr, &len); 
						buffer[n] = '\0'; 
						printf("Client : %s\n", buffer); 
						sendto(sfd[i], (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &c_addr,len); 
						printf("Hello message sent.\n"); 
						int fds[1]={sfd[i]};
						send_fd(usfd,fds,1);
						cout<<"sent"<<endl;
						cout<<fds[0]<<endl;
    		
					}
					
				}
					
			}
		}
		
	}
}



int main(int argc, const char* argv[]) 
{ 
	
    int  nsfd;
    int valread;
    int port[3];
    port[0]=8080; 	//c3
    port[1]=8085;	//c4
    port[2]=7020;	//c5
    struct sockaddr_un addr;
    
    
    
    
    
    
    	usfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (usfd == -1)
        handle_error ("Failed to create socket");

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path)-1);
        if (connect(usfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
                handle_error ("Failed to connect to socket");
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    	FD_SET(sfd[0],&rfds);
	FD_SET(sfd[1],&rfds);
	FD_SET(sfd[2],&rfds);
	int ma=-1;
	for(int i=0;i<3;i++)
	{
		if(ma<sfd[i])
	    	ma=sfd[i];
	}
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    int m=-1;   
    
   	if ((sfd[0] = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (setsockopt(sfd[0], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   		address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( port[0] ); 
    	if (bind(sfd[0], (struct sockaddr *)&address,sizeof(address))<0) 
    	{ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (listen(sfd[0], 3) < 0) 
    	{ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    	}
    	if(m<sfd[0])
    	m=sfd[0];
    	
    	
    	
    	
	
	
	
	if ( (sfd[1] = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&s_addr, 0, sizeof(s_addr)); 
	memset(&c_addr, 0, sizeof(c_addr)); 
	s_addr.sin_family = AF_INET; 
	s_addr.sin_addr.s_addr = INADDR_ANY; 
	s_addr.sin_port = htons(port[1]); 
	if ( bind(sfd[1], (const struct sockaddr *)&s_addr, sizeof(s_addr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if(m<sfd[1])
    	m=sfd[1];
	
	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	if ((sfd[2] = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (setsockopt(sfd[2], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   		address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( port[2] ); 
    	if (bind(sfd[2], (struct sockaddr *)&address,sizeof(address))<0) 
    	{ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (listen(sfd[2], 3) < 0) 
    	{ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    	}
    	if(m<sfd[2])
    	m=sfd[2];
    	
    	
    
   	
   	
	pthread_create(&tid,NULL,func,NULL);
	pthread_join(tid,NULL);
	while(1);
	
	close(sfd[0]);
	close(sfd[1]);
	close(sfd[2]);
	return 0;
}









/*

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include<bits/stdc++.h>
using namespace std;
static int cnt=0;
int A[3]={0};
int sfd[3];
void* func1(void * arg)
{
	cout<<"func1"<<endl;
	int * fd=(int *)arg;
	int nsfd=*fd;
    	char buffer[1024]; 
    	cout<<nsfd<<endl;
    	cout<<"train reached platform";
    	sleep(1);
    	cnt--;
    	A[0]=0;
    	pthread_exit((void*)0);
}
void* func2(void * arg)
{
	cout<<"func2"<<endl;
	int * fd=(int *)arg;
	int nsfd=*fd;
    	char buffer[1024]; 
    	cout<<nsfd<<endl;
    	cout<<"train reached platform";
    	sleep(1);
    	cnt--;
    	A[1]=0;
    	pthread_exit((void*)0);
}
void* func3(void * arg)
{
	cout<<"func3"<<endl;
	int * fd=(int *)arg;
	int nsfd=*fd;
    	char buffer[1024]; 
    	cout<<nsfd<<endl;
    	cout<<"train reached platform";
    	sleep(1);
    	cnt--;
    	A[2]=0;
    	pthread_exit((void*)0);
}
int main(int argc, const char* argv[]) 
{ 
	
    int  nsfd;
    int valread;
    int port[3];
    port[0]=8080;
    port[1]=8085;
    port[2]=7020;
    pthread_t tid0,tid1,tid2;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};    
    for(int i=0;i<3;i++)
    {
   	 if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   	address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( port[i] ); 
    	if (bind(sfd[i], (struct sockaddr *)&address,sizeof(address))<0) 
    	{ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    	} 
    	if (listen(sfd[i], 3) < 0) 
    	{ 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    	}
    }
   
   	struct pollfd pfds[3];
	pfds[0].fd=sfd[0];
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	pfds[1].fd=sfd[1];
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	pfds[2].fd=sfd[2];
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	while(1)
	{
		int count = poll(pfds,3,5000);
		if(count==-1)
		perror("poll error");
		else if(count==0)
		{printf("time out\n");}
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(cnt<3)
				{	
					cnt++;
					cout<<cnt<<endl;
					if(pfds[i].revents && POLLIN)
					{	
   						 if ((nsfd = accept(pfds[i].fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    						 { 
        						 perror("accept"); 
        						 exit(EXIT_FAILURE); 
    						 } 
    						pfds[i].revents=0;
    						if(A[0]==0)
						{
							A[0]=1;
							cout<<"hi1"<<endl;
							pthread_create(&tid0,NULL,func1,&nsfd);
							
							pthread_join(tid0,NULL);
						}
						else if(A[1]==0)
						{
							A[1]=1;
							cout<<"hi2"<<endl;
							pthread_create(&tid1,NULL,func2,&nsfd);
							
							pthread_join(tid1,NULL);
						}
						else if(A[2]==0)
						{
							A[2]=1;
							cout<<"hi3"<<endl;
							pthread_create(&tid2,NULL,func3,&nsfd);
							
							pthread_join(tid2,NULL);
						}
						
					}
					else
					free++;
				}
				else
				{
					cout<<"platforms are full"<<endl;
				
				}	
			}
			if(free == 3)
			cout<<"timeout"<<endl;
		}
		pfds[0].fd=sfd[0];
		pfds[0].events=POLLIN;
		pfds[0].revents=0;
		pfds[1].fd=sfd[1];
		pfds[1].events=POLLIN;
		pfds[1].revents=0;
		pfds[2].fd=sfd[2];
		pfds[2].events=POLLIN;
		pfds[2].revents=0;
	}
	close(sfd[0]);
	close(sfd[1]);
	close(sfd[2]);
	return 0;
}


*/

