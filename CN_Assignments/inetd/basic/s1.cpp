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

int main()
{  
    	char buffer[1024]; 
    	int valread;
    	int fd;
    	dup2(0,fd);
    	valread = recv(2,buffer,1024,0); 
	cout<<buffer<<endl; 
	string s="s1";
	char *hello = &s[0]; 
	send(2, hello , s.length() , 0 ); 
	   
    
    
    
    
    
    
    
    
} 
