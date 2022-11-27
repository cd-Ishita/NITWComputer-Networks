#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <bits/stdc++.h>
using namespace std;
#define PING_PKT_S 64
#define PORT_NO 0
#define PING_SLEEP_RATE 1000000
#define RECV_TIMEOUT 1
int pingloop=1;
struct ping_pkt
{
	struct icmphdr hdr;
	char msg[PING_PKT_S-sizeof(struct icmphdr)];
};
void intHandler(int dummy)
{
	pingloop=0;
}
char *add_copy(char *addr_host, struct sockaddr_in *addr_con)
{
	struct hostent *host_entity;
	char *ip=(char*)malloc(1024*sizeof(char));
	host_entity = gethostbyname(addr_host);
	strcpy(ip, inet_ntoa(*(struct in_addr *)host_entity->h_addr));
	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons (PORT_NO);
	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;
	return ip;	
}

char* buff_convert(char *ip)
{
	struct sockaddr_in addr;	
	socklen_t len;
	char buf[1024], *ret_buf;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	len = sizeof(struct sockaddr_in);
	getnameinfo((struct sockaddr *) &addr, len, buf,sizeof(buf), NULL, 0, NI_NAMEREQD);
	ret_buf = (char*)malloc((strlen(buf) +1)*sizeof(char) );
	strcpy(ret_buf, buf);
	return ret_buf;
}

void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr,char *ping_dom, char *ping_ip, char *rev_host)
{
	int ttl_val=64, msg_count=0, i, addr_len, flag=1,msg_received_count=0;
	struct ping_pkt pckt;
	struct sockaddr_in addr;
	struct timespec time_start, time_end, tfs, tfe;
	long double rtt_msec=0, total_msec=0;
	struct timeval tv_out;
	tv_out.tv_sec = RECV_TIMEOUT;
	tv_out.tv_usec = 0;
	clock_gettime(CLOCK_MONOTONIC, &tfs);
	setsockopt(ping_sockfd, SOL_IP, IP_TTL,&ttl_val, sizeof(ttl_val));
	setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO,(const char*)&tv_out, sizeof tv_out);
	while(pingloop)
	{
		flag=1;
		bzero(&pckt, sizeof(pckt));
		pckt.hdr.type = ICMP_ECHO;
		pckt.hdr.un.echo.id = getpid();
		for ( i = 0; i < sizeof(pckt.msg)-1; i++ )
			pckt.msg[i] = i+'0';
		pckt.msg[i] = 0;
		pckt.hdr.un.echo.sequence = msg_count++;
		usleep(PING_SLEEP_RATE);
		clock_gettime(CLOCK_MONOTONIC, &time_start);
		if ( sendto(ping_sockfd, &pckt, sizeof(pckt), 0,
		(struct sockaddr*) ping_addr,
			sizeof(*ping_addr)) <= 0)
		{
			printf("\nPacket Sending Failed!\n");
			flag=0;
		}
		socklen_t addr_len=sizeof(addr);
		if ( recvfrom(ping_sockfd, &pckt, sizeof(pckt), 0,(struct sockaddr*)&addr, &addr_len) <= 0&& msg_count>1)
		{
			printf("\nPacket receive failed!\n");
		}

		else
		{
			clock_gettime(CLOCK_MONOTONIC, &time_end);
			
			double timeElapsed = ((double)(time_end.tv_nsec -time_start.tv_nsec))/1000000.0;
			rtt_msec = (time_end.tv_sec-time_start.tv_sec) * 1000.0+ timeElapsed;
			if(flag)
			{
				if(!(pckt.hdr.type ==69 && pckt.hdr.code==0))
				{
					printf("Error..Packet received with ICMP type %d code %d\n",pckt.hdr.type, pckt.hdr.code);
				}
				else
				{
					printf("%d bytes from %s (h: %s) (%s) msg_seq=%d ttl=%d rtt = %Lf ms.\n",PING_PKT_S,ping_dom,rev_host,ping_ip,msg_count,ttl_val, rtt_msec);

					msg_received_count++;
				}
			}
		}	
	}
	clock_gettime(CLOCK_MONOTONIC, &tfe);
	double timeElapsed = ((double)(tfe.tv_nsec -tfs.tv_nsec))/1000000.0;
	
	total_msec = (tfe.tv_sec-tfs.tv_sec)*1000.0+timeElapsed;
					
	printf("\n===%s ping statistics===\n", ping_ip);
	printf("\n%d packets sent, %d packets received, %f percent packet loss. Total time: %Lf ms.\n\n",msg_count, msg_received_count,((msg_count-msg_received_count)/msg_count) * 100.0,total_msec);
}

int main()
{
	int rsfd;
	char *ip, *buff;
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	char buffer[1024];
	char* s="www.google.com";
	ip = add_copy(s, &addr);
	buff = buff_convert(ip);
	rsfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	signal(SIGINT, intHandler);
	send_ping(rsfd, &addr, buff,ip, s);
	return 0;
}

