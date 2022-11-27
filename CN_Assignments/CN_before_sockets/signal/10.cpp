#include<stdio.h>
#include<signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include<fcntl.h>
void p2c(int c)
{
printf("p-c\n");
//signal(SIGUSR1,SIG_DFL);
}
void c2p(int c)
{
printf("c-p\n");
//signal(SIGUSR2,SIG_DFL);
}
int main()
{
//signal(SIGUSR1,SIG_DFL);
signal(SIGUSR1,p2c);
signal(SIGUSR2,c2p);
int c;
c=fork();
if(c>0)
{
	
//	printf("kill child\n");
	kill(c,SIGUSR1);
	//raise(SIGUSR1);
}
else
{
	sleep(10);
//	sleep(1);
//	printf("kill parent\n");
	kill(getppid(),SIGUSR2);
}
return 0;
}
