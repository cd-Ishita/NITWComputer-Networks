#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

#include <bits/stdc++.h>
using namespace std;

int main()
{
	int nsfd=dup(0);
	dup2(65,0);
	dup2(66,1);
	char buffer[1024] = {0}; 
	int valread = recv( nsfd , buffer, 1024,0); 
	cout<<buffer<<endl;
	string s="";
	cout<<"wassup2"<<endl;
	cin>>s;
	char *hello = &s[0]; 
	send(nsfd, hello , s.length() , 0 ); 

}
