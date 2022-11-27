#include"header.h"

int main(int argc, char const *argv[])
{
	int rsfd = 	socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	perror("socket : ");
	cout<<"\n";

	unsigned char * packet = (unsigned char *) malloc(65536);
	memset(packet,0,65536);
	struct sockaddr saddr;
	int saddr_len = sizeof(saddr);

	int packetlen = recvfrom(rsfd,packet,65536,0,&saddr,(socklen_t *)&saddr_len);
	perror("recvfrom : ");
	cout<<"\n";

	struct ethhdr *eth;
    eth = (struct ethhdr *) packet;
	print_ethernet_header(packet,packetlen);

	fflush(logfile);

	print_ip_header(packet,packetlen);

	fflush(logfile);

	// if(iph->protocol == 6)
		print_tcp_packet(packet,packetlen);	

	// else if(ip->protocol == 14)
		print_udp_packet(packet,packetlen);

	fflush(logfile);

	unsigned char * data = (packet + sizeof(iphdr) + sizeof(struct ethhdr) + sizeof(struct udphdr));

	PrintData(data,sizeof(data));

	fflush(logfile);

	return 0;
}
