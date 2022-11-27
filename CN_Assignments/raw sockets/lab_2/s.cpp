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
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<bits/stdc++.h>
using namespace std;

#define BUF_LEN 1024

int main()
{
	int rsfd1 = socket (PF_INET, SOCK_RAW, 1);
	int rsfd2 = socket (PF_INET, SOCK_RAW, 2);
	int rsfd3 = socket (PF_INET, SOCK_RAW, 3);
	int n = 0 ;
	int one = 1;
    	const int *val = &one;
    	if (setsockopt (rsfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    	cout<<"Not set";
    	char buff[4096];
    	struct iphdr *iph = (struct iphdr *)buff;
    	struct sockaddr_in sin,sin2; 
    	socklen_t len = sizeof(sin2);
   	memset(&sin,0,sizeof(sin));
    	sin.sin_family = AF_INET;
    	sin.sin_port = htons (6000);
    	sin.sin_addr.s_addr = htonl(INADDR_ANY);
    	bind(rsfd,(struct sockaddr *) &sin,sizeof(sin));
    	
    	
    	
    	int sfd[3];
  	sfd[0]=rsfd1;
  	sfd[1]=rsfd2;
  	sfd[2]=rsfd3;
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
		
			for(int i=0;i<3;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{	
					recvfrom(sfd[i],buff,4096,0,(struct sockaddr *) &sin,&len);
					if()	// 1st time
					{	iph->protocol =  i+1;
						sendto(s,buff,iph->tot_len,0,(struct sockaddr *) &sin,sizeof (sin));
					
					}
						
					else
					{
						// yes or no
					
						sendto(s,buff,iph->tot_len,0,(struct sockaddr *) &sin,sizeof (sin));
					
					}
				}
			
			}
			
					
		}
		
	}

    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	
    	while(1)
    	{	
    		
    		
    		break;
    	
    	}
    	
    	
}
