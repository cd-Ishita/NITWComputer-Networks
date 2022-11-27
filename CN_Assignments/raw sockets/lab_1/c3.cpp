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

void print_ipheader(struct iphdr* ip)
{
	cout<<"------------------------\n";
	cout<<"Printing IP header....\n";
	cout<<"IP version:"<<(unsigned int)ip->version<<endl;
	cout<<"IP header length:"<<(unsigned int)ip->ihl<<endl;
	
	cout<<"Type of service:"<<(unsigned int)ip->tos<<endl;
	cout<<"Total ip packet length:"<<ntohs(ip->tot_len)<<endl;
	cout<<"Packet id:"<<ntohs(ip->id)<<endl;
	cout<<"Time to leave :"<<(unsigned int)ip->ttl<<endl;
	cout<<"Protocol:"<<(unsigned int)ip->protocol<<endl;
	cout<<"Check:"<<ip->check<<endl;
	cout<<"Source ip:"<<inet_ntoa(*(in_addr*)&ip->saddr)<<endl;
	cout<<ip->saddr<<endl;
	
	//printf("%pI4\n",&ip->saddr );
	cout<<"Destination ip:"<<inet_ntoa(*(in_addr*)&ip->daddr)<<endl;
	cout<<"End of IP header\n";
	cout<<"------------------------\n";
}

void reverse(char* str)
{
    // l for swap with index 2
    int l = 2;
    int r = strlen(str) - 2;
  
    // swap with in two-2 pair
    while (l < r) {
        swap(str[l++], str[r++]);
        swap(str[l++], str[r]);
        r = r - 3;
    }
}
  
// function to conversion and print
// the hexadecimal value
void ipToHexa(int addr)
{
    char str[15];
  
    // convert integer to string for reverse
    sprintf(str, "0x%08x", addr);
  
    // reverse for get actual hexadecimal
    // number without reverse it will
    // print 0x0100007f for 127.0.0.1
    reverse(str);
  
    // print string
    cout << str << "\n";
}


int main()
{
	int rsfd = socket (PF_INET, SOCK_RAW, 1);
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
    	vector<int>v;
    	while(1)
    	{	
    		recvfrom(rsfd,buff,4096,0,(struct sockaddr *) &sin,&len);
    		cout<<sin.sin_port<<endl;
    		cout<<(char*)buff<<endl;
    		struct iphdr *ipp;
		ipp=(struct iphdr*)buff;
		int p=0;
		string s=inet_ntoa(*(in_addr*)&ipp->saddr);
		string str="";
		int a=0;
		for(int i=0;s[i];i++)
		{	
			
			
			if(s[i]!='.')
			{
				str+=s[i];
			}
			else
			{
				p=p*256+stoi(str);
				str="";
			}
			
		}
		p=p*256+stoi(str);
		cout<<p<<endl;
		v.push_back(p);
		
		break;
    	
    	}
    	int sfd = 0, valread; 
	struct sockaddr_in address; 
	
	char buffer[1024] = {0}; 
	if ((sfd= socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	address.sin_family = AF_INET; 
	address.sin_port = htons(v[0]); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sfd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
//	while(1){
	string s="c3";
	char *hello = &s[0]; 
	send(sfd, hello , s.length() , 0 ); 
	valread = recv( sfd , buffer, 1024,0); 
	cout<<buffer<<endl;
	
	
//	}
	
    	
}
