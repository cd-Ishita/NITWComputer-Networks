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
#include<bits/stdc++.h>
#include <fstream>
using namespace std;

static struct sigaction siga;
key_t key=ftok(".",65);

struct shmem
{
	int i;
};
struct shmem *z;
int k;


int f=1;

int counter=1;

static void multi_handler(int sig, siginfo_t *siginfo, void *context) 
{
    // get pid of sender,

  //  if(sig==SIGUSR2)
  //  {
  //  	f=0;
   // }
 //   else
   // {
   counter=1;;

    	pid_t sender_pid = siginfo->si_pid;
        cout<<"signal from    "<<sender_pid<<endl;
      
   //     kill((pid_t)k,SIGUSR2);
  // }
}
fd_set rfds;
 int cnt=0;
int main() 
{
	cout<<"process of p22:"<<getpid()<<endl;
	siga.sa_sigaction = *multi_handler;
    	siga.sa_flags |= SA_SIGINFO;
    	sigaction(SIGUSR1, &siga, NULL);
    	
    	int shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
    	z = (struct shmem*)shmat(shmid,NULL,0);
    	z->i=getpid();
	int hh;
	cout<<"enter a integer to start:"<<endl;
	cin>>hh;
	cout<<"hi"<<endl;
	ifstream fin;		
 	
 	
 	
 	while(1)
 	{

 			int sfd[20], nsfd;
    			int valread;
    			int port[20];
    			int limit[20];
    			string path[20];
    			string con[20];
    
    			struct sockaddr_in address; 
    			int opt = 1; 
    			socklen_t addrlen = sizeof(address); 
    			
    			string s;
    			fin.open("config.txt");
 		if(counter)
 		{
 			counter=0;
 			
    			cnt=0;
			    while(getline(fin,s))
			    {
			    	stringstream ss(s);
			    	ss>>port[cnt];
			    	ss>>con[cnt];
			    	ss>>limit[cnt];
			    	ss>>path[cnt];
    				cout<<port[cnt];
    				cout<<con[cnt];
    				cout<<limit[cnt];
    				cout<<path[cnt];
    				cout<<endl;
    	
    	
    				if ((sfd[cnt] = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    				{ 
    				    perror("socket failed"); 
        				exit(EXIT_FAILURE); 
    				} 
    	
    	
    				if (setsockopt(sfd[cnt], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    				{ 
        				perror("setsockopt"); 
        				exit(EXIT_FAILURE); 
    				} 
    				address.sin_family = AF_INET; 
   				address.sin_addr.s_addr = INADDR_ANY; 
    				address.sin_port = htons( port[cnt] ); 
    	
    	
    				if (bind(sfd[cnt], (struct sockaddr *)&address,sizeof(address))<0) 
    				{ 
        				perror("bind failed"); 
       				 exit(EXIT_FAILURE); 
    				} 
    				if (listen(sfd[cnt],limit[cnt] ) < 0) 
    				{ 
        				perror("listen"); 
        				exit(EXIT_FAILURE); 
    				}
    
    				cnt++;
    			}
    			
 		
 		}
 	   while(1)
	   {
	   	
		if(counter>0)
		{
			cout<<"counter: "<<counter<<endl;
			break;
		
		}
		
		FD_ZERO(&rfds);
		for(int i=0;i<cnt;i++)
		FD_SET(sfd[i],&rfds);
		int ma=-1;
		for(int i=0;i<cnt;i++)
		{
			if(ma<sfd[i])
	    		ma=sfd[i];
	    	}
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		if(count>0)
		{	cout<<"count: "<<count<<endl;
			for(int i=0;i<cnt;i++)
			{
				if(FD_ISSET(sfd[i],&rfds))
				{	cout<<"hi"<<endl;
					int nsfd;
   					 if ((nsfd = accept(sfd[i], (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
    					 { 
        					 perror("accept"); 
        					 exit(EXIT_FAILURE); 
    					 } 
    					 int c=fork();
    					 cout<<"c: "<<c<<endl;
    					 if(c>0)
    					 {	
    					 	close(nsfd);
    					 	
    					}
    					else
    					{
    						for(int j=0;j<cnt;j++)	
					 	close(sfd[j]);
					    	dup2(nsfd,2);
					   // 	cout<<"exec: "<<path[i]<<endl;
					    	const char* p=path[i].c_str();
    					    	execv(p,NULL);
    					
    					}
    						
				}
			
			}	
		}	
	    }
 	}
    
    
    
    
    
    	
}

