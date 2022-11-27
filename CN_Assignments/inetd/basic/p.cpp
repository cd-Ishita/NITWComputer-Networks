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
	ofstream fout;		
 	fout.open("config.txt");
 	int n;
 	cin>>n;
 	for(int i=0;i<n;i++)
 	{
 		string s=to_string(i+1);
 		s="./s"+s;
 		fout<<8080+100*i<<"\t"<<"tcp\t"<<10<<"\t"<<s<<endl;
 	
 	
 	}



}
