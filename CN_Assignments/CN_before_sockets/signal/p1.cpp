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



void handler(int count) {
	cout<<"I won't terminate \n";
	signal(SIGINT,handler);
}



int main() {
	signal(SIGINT, handler);
	while(1)
	{
	//	signal(SIGINT, handler);
		
		cout<<"enter ctrl + c"<<endl;
		sleep(1);
	}
	
	


	return 0;
}
