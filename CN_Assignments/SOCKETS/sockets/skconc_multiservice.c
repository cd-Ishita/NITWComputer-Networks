#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
static int cnt=0;
int main(int argc, const char* argv[]) 
{ 
    int sfd[3], nsfd;
    int valread;
    int port[3];
    port[0]=8080;
    port[1]=8085;
    port[2]=7020; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};    
    // Creating socket file descriptor 
    for(int i=0;i<3;i++)
    {
   	 if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    	{ 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    	} 
       
    // Forcefully attaching socket to the port 8080 
    	if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    	{ 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    	} 
    	address.sin_family = AF_INET; 
   	address.sin_addr.s_addr = INADDR_ANY; 
    	address.sin_port = htons( port[i] ); 
       
    // Forcefully attaching socket to the port 8080 
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
	//cout<<"Poling is Ready to start"<<endl;
	while(1)
	{
		int count = poll(pfds,3,5000);
		//cout<<count<<endl;
		if(count==-1)
		perror("poll error");
		else if(count==0)
		{printf("time out\n");
		break;}
		else
		{
			int c=0;
			int free=0;
			for(int i=0;i<3;i++)
			{
				char ch[120];
				if(pfds[i].revents && POLLIN  )
				{	
   				 if ((nsfd = accept(pfds[i].fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    				 { 
        			 perror("accept"); 
        			 exit(EXIT_FAILURE); 
    				 } 
    				 int c=fork();
    				 if(c==0)
    				 {
				    close(pfds[i].fd);
				    dup2(nsfd,0);
    				    if(i==0)
 				    execv("./sforc1",argv);
				    else if(i==1)
    				    {
				    execv("./sforc2",argv);
    				    }
				    else
 				    {
       			     execv("./sforc3",argv);
    				    }
    				}
				pfds[i].revents=0;
				}
				else
				free++;
			}
			if(free == 3)
			break;
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

