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
int f=1;
void handler(int count) 
{
	sleep(1);
	cout<<"recieved signal from child my pid ="<<getpid()<<" my grp pid ="<<getpgrp()<<endl;
	f=0;
}



int main() 
{
	int c=fork();
	for(int i=0;i<4;i++)
	{
		if(c>0)
		c=fork();
		else 
		break;
		
	}
	if(c>0)
	{
		sleep(2);
		signal (SIGUSR1,SIG_IGN);	//parent will ignore signal
		cout<<"parent's grp id is= "<<getpgrp()<<endl;
		killpg(getpgrp(),SIGUSR1);
	}
	else
	{
		signal (SIGUSR1,handler);	// child will handle signal
		pid_t ppgid=getpgid(getppid());
		cout<<"in child's parent grp is "<<ppgid<<endl;
		setpgid(getpid(),ppgid);// grouping
		while(f);
	}

	return 0;
}
