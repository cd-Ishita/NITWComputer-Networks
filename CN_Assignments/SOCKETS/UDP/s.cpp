#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <bits/stdc++.h> 
using namespace std;

#define PORT	 8080 
#define MAXLINE 1024 

int main() { 
	int sfd; 
	char buffer[MAXLINE]; 
	char *hello = "Hello from server"; 
	struct sockaddr_in s_addr, c_addr; 
	
	
	if ( (sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&s_addr, 0, sizeof(s_addr)); 
	memset(&c_addr, 0, sizeof(c_addr)); 
	s_addr.sin_family = AF_INET; 
	s_addr.sin_addr.s_addr = INADDR_ANY; 
	s_addr.sin_port = htons(PORT); 
	if ( bind(sfd, (const struct sockaddr *)&s_addr, sizeof(s_addr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int n; 
	socklen_t len = sizeof(c_addr);
	n = recvfrom(sfd, (char *)buffer, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &c_addr, &len); 
	buffer[n] = '\0'; 
	printf("Client : %s\n", buffer); 
	sendto(sfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &c_addr,len); 
	printf("Hello message sent.\n"); 
	
	return 0; 
} 

