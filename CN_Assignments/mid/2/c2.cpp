// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include <bits/stdc++.h>
using namespace std;
#define PORT 8080

int main(int argc, char const *argv[]) 
{ 
	cout<<"hic2"<<endl;
	int sfd = 0, valread; 
	struct sockaddr_in address; 
	
	char buffer[1024] = {0}; 
	if ((sfd= socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	address.sin_family = AF_INET; 
	address.sin_port = htons(PORT); 
	
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
	int n=4;
	while(n--){
	string s="c2";
	char *hello = &s[0]; 
	send(sfd, hello , s.length() , 0 ); 
	cout<<"sent"<<endl;
	
	}
	
	return 0; 
} 

