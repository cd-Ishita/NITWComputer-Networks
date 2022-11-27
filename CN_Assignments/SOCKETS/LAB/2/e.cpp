#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
using namespace std;

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
int main()
{
	key_t key = ftok("shmfile",65); 
    	int shmid = shmget(key,1024,0666|IPC_CREAT); 
    	char *str = (char*) shmat(shmid,(void*)0,0); 
  
    printf("Data read from memory: %s\n",str); 
	struct sockaddr_un addr;
	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sfd == -1)
        handle_error ("Failed to create socket");

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path)-1);
	FD_ZERO(&rfds);
	FILE *fd[0]=popen("./r1","r");
	FILE *fd[1]=popen("./r2","r");
	FILE *fd[2]=popen("./r3","r");
	while(1)
	{
	
		for(int i=0;i<3;i++)
		FD_SET(fd[i],&rfds);
		int ma=-1;
		for(int i=0;i<3;i++)
		{
			if(ma<fd[i])
		    	ma=fd[i];
		}
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		if(count>0)
		{
			for(int i=0;i<3;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{
					
					
				
				
				
				}	
			}
		
		}
	
	
	
	
	
	
	}
	
	
	
	
	
	
	
	
	
	
	struct pollfd pfds[3];
	cout<<"Enter Value for P1 : "<<endl;
	
	wait(NULL);
	pfds[0].fd=fileno(fd1);
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	cout<<"Enter Value for P2 : "<<endl;
	FILE *fd2=popen("./p2","r");
	wait(NULL);
	pfds[1].fd=fileno(fd2);
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	cout<<"Enter Value for P3 : "<<endl;
	FILE *fd3=popen("./p3","r");
	wait(NULL);
	pfds[2].fd=fileno(fd3);
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	cout<<"Poling is Ready to start"<<endl;
	while(1)
	{
		int count = poll(pfds,3,500);
		cout<<count<<endl;
		if(count==-1)
		perror("poll error");
		else if(count==0)
		printf("time out\n");
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(pfds[i].revents && POLLIN && (c=read(pfds[i].fd,ch,120))>0)
				{	
					write(1,ch,c);
					cout<<"\t From Process "<<i+1<<endl;
					pfds[i].revents=0;
				}
				else
				free++;
			}
			if(free == 3)
			break;
		}
	cout<<"Enter Value for P1 : "<<endl;
	FILE *fd1=popen("./p1","r");
	wait(NULL);
	pfds[0].fd=fileno(fd1);
	pfds[0].events=POLLIN;
	pfds[0].revents=0;
	cout<<"Enter Value for P2 : "<<endl;
	FILE *fd2=popen("./p2","r");
	wait(NULL);
	pfds[1].fd=fileno(fd2);
	pfds[1].events=POLLIN;
	pfds[1].revents=0;
	cout<<"Enter Value for P3 : "<<endl;
	FILE *fd3=popen("./p3","r");
	wait(NULL);
	pfds[2].fd=fileno(fd3);
	pfds[2].events=POLLIN;
	pfds[2].revents=0;
	}
	
	pclose(fd1);
	pclose(fd2);
	pclose(fd3);
	return 0;
}
