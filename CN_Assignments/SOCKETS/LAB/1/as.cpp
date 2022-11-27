#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <bits/stdc++.h>
#include <sys/un.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include<arpa/inet.h>
#include <sys/un.h>
#include<poll.h>
using namespace std;

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while(0)

static
int * recv_fd(int socket, int n) 
{
	
        int *fds=new int[1];
        struct msghdr msg = {0};
        struct cmsghdr *cmsg;
        char buf[CMSG_SPACE(n * sizeof(int))], dup[256];
        memset(buf,'\0', sizeof(buf));
        struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);

        if (recvmsg (socket, &msg, 0) < 0)
                handle_error ("Failed to receive message");

        cmsg = CMSG_FIRSTHDR(&msg);

        memcpy (fds, (int *) CMSG_DATA(cmsg), n * sizeof(int));

        return fds;
}

int
main(int argc, char *argv[]) {
        ssize_t nbytes;
        char buffer[256]={"hiiii"};
        int sfd, cfd;
        struct sockaddr_un addr;

        sfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sfd == -1)
                handle_error ("Failed to create socket");

        if (unlink ("/tmp/fd-pass.socket") == -1 && errno != ENOENT)
                handle_error ("Removing socket file failed");

        memset(&addr, 0, sizeof(struct sockaddr_un));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, "/tmp/fd-pass.socket", sizeof(addr.sun_path)-1);

        if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
                handle_error ("Failed to bind to socket");

        if (listen(sfd, 5) == -1)
                handle_error ("Failed to listen on socket");

        cfd = accept(sfd, NULL, NULL);
        if (cfd == -1)
                handle_error ("Failed to accept incoming connection");
	
	
	
	
	
	
	
	int cnt=10;
	while(cnt>0)
	{
		int * rfd;
		int y;
		if((rfd = recv_fd(cfd,1))< 0) 
		{
        			perror("recv_fd");
        		exit(1);
    		}
    		struct sockaddr_in xyz,address;
    		int sd=sizeof(xyz); 
    		getsockname(rfd[0],(struct sockaddr *)&xyz,(socklen_t*)&sd);
		printf("port=%d\n",ntohs(xyz.sin_port));
    		if(ntohs(xyz.sin_port)==8080||ntohs(xyz.sin_port)==7020)
    		{
    			char buffer[1024] = {0}; 
			char hello[100]; 
    			recv( rfd[0] , buffer, 1024,0); 
    			printf("%s\n",buffer); 
   			for(int i=0;i<strlen(buffer);i++)
   			{
   				if(buffer[i]>='a' && buffer[i]<='z')
   				buffer[i]=buffer[i]-'a'+'A';
   			}
    			send(rfd[0],buffer,strlen(buffer),0 ); 
   			printf("Hello message sent\n");
    		}
    		if(ntohs(xyz.sin_port)==8085)
    		{
    			char buffer[1024] = {0}; 
			socklen_t addrlen=sizeof(address);
    		//	if(recvfrom( rfd[0] , buffer, 1024,0,(struct sockaddr *)&address, &addrlen)>0)
    			cout<<"hi"<<endl;
    			printf("%s\n",buffer); 
   			for(int i=0;i<strlen(buffer);i++)
   			{
   				if(buffer[i]>='A' && buffer[i]<='Z')
   				buffer[i]=buffer[i]-'A'+'a';
   			}
    			if(sendto(rfd[0],buffer,strlen(buffer),0,(struct sockaddr *)&address,sizeof(address))>0)
    			cout<<"hi"<<endl;
   			printf("Hello message sent%s\n",buffer);
    		}
    		
    		cnt--;
 //   		close(nufd);
    	}

	
	
	
	
	
	
	/*
	
	
	while(1){
		fds = recv_fd (cfd, 1);
		cout<<fds[0]<<endl;
               if(write(fds[0], buffer,5)>0)
               cout<<"written"<<endl;
                
        
	
	
	
	}

        if (close(cfd) == -1)
                handle_error ("Failed to close client socket");
                */

        return 0;
}
