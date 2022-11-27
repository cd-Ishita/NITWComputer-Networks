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
	//printf("%pI4\n",&ip->saddr );
	cout<<"Destination ip:"<<inet_ntoa(*(in_addr*)&ip->daddr)<<endl;
	cout<<"End of IP header\n";
	cout<<"------------------------\n";
}



string getIP(unsigned int IP)
{
    unsigned char *p = (unsigned char *)&IP;
    return to_string(p[0]) + "." + to_string(p[1]) + "." + to_string(p[2]) + "." + to_string(p[3]);
}

void print(ip I)
{
    cout << "Version: " << (int)I.ip_v << endl;
    cout << "Header Length: " << (int)I.ip_hl << endl;
    cout << "Type of Service: " << (int)I.ip_tos << endl;
    cout << "Packet Length: " << (int)I.ip_len << endl;
    cout << "ID: " << (int)I.ip_id << endl;
    cout << "Fragment: " << (int)I.ip_off << endl;
    cout << "Time to Live: " << (int)I.ip_ttl << endl;
    cout << "Higher Level Protocol: " << (int)I.ip_p << endl;
    cout << "CheckSum: " << (int)I.ip_sum << endl;
    cout << "Sender: " << getIP(I.ip_src.s_addr) << endl;
    cout << "Receiver: " << getIP(I.ip_dst.s_addr) << endl;
}


int main()
{
	int rsfd=socket(AF_INET,SOCK_RAW,IPPROTO_UDP);
	perror("socket");
	int optval=1;
	setsockopt(rsfd, IPPROTO_IP, SO_BROADCAST, &optval, sizeof(int));//IP_HDRINCL
	cout<<"opt"<<endl;
	struct sockaddr_in client;
	client.sin_family=AF_INET;
	client.sin_addr.s_addr=inet_addr("127.0.0.1");

	char buff[]="hello";	
	client.sin_addr.s_addr=INADDR_ANY;

	unsigned int client_len=sizeof(client);
	cout<<"sending"<<endl;
  	sendto(rsfd,buff,strlen(buff)+1,0,(struct sockaddr*)&client,sizeof(client));
  	perror("send");
  	
  	 struct iphdr *ipp;
  	ipp=(struct iphdr*)buff;
  	ip I = *(ip *)buff;
  	 print_ipheader(ipp);
  	 print(I);


}

