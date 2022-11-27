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
	cout<<"I RAISED \n";
}



int main() {
	signal(SIGUSR1, handler);
//	raise(SIGUSR1);
	kill(getpid(),SIGINT);	
	
	


	return 0;
}
