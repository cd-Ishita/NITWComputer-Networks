#include <ctype.h>
#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
using namespace std;
void p_c_handler(int count) 
{
	cout<<"recieved from parent"<<endl;
}
void c_p_handler(int count) 
{
	cout<<"recieved from child"<<endl;
}
int main() 
{
	signal(SIGUSR1,p_c_handler);
	signal(SIGUSR2,c_p_handler);
	int c= fork();
	if(c>0)
	{
		sleep(1);
		kill(c,SIGUSR1);
	
	}
	else
	{
		sleep(1);
		kill(getppid(),SIGUSR2);
	
	}
	
	
	


	return 0;
}
