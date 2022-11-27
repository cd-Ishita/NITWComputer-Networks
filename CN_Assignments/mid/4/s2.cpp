#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<bits/stdc++.h>
#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include<bits/stdc++.h>
using namespace std;
int nsfd=dup(0);
void* service(void* arg)
{	
	
	while(1)
	{
		char buffer[1024]={'\0'};
		recv(nsfd,buffer,1024,0);
		cout<<buffer<<endl;
		string s="recieved_s2";
		char *hello = &s[0]; 
		send(nsfd, hello , s.length() , 0 ); 
	
	}
}


int main()
{

	
	
	pthread_t t1;
	pthread_create(&t1,NULL,service,NULL);
	pthread_join(t1,NULL);
}
