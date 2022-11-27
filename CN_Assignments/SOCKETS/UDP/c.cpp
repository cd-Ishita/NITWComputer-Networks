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
	char *hello = "Hello from client"; 
	struct sockaddr_in s_addr; 

	if ( (sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&s_addr, 0, sizeof(s_addr)); 
	s_addr.sin_family = AF_INET; 
	s_addr.sin_port = htons(PORT); 
	s_addr.sin_addr.s_addr = INADDR_ANY; 
	
	int n;
	socklen_t len; 
	
	sendto(sfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &s_addr, sizeof(s_addr)); 
	printf("Hello message sent.\n"); 
		
	n = recvfrom(sfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &s_addr, &len); 
	buffer[n] = '\0'; 
	printf("Server : %s\n", buffer); 

	close(sfd); 
	return 0; 
} 

