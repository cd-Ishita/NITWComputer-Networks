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
pthread_t t1,t2;


void handler1(int count)
{
	cout<<"thread1"<<endl;
	sleep(1);
	pthread_kill(t2,SIGUSR2);



}
void handler2(int count)
{
	
	cout<<"thread2"<<endl;
	sleep(1);
	pthread_kill(t1,SIGUSR1);



}

void* f1(void * arg)
{
	signal(SIGUSR1,handler1);
	while(1);


}
void* f2(void * arg)
{
	signal(SIGUSR2,handler2);
	while(1);

}


int main()
{
	cout<<"hi"<<endl;
	int arg;

	pthread_create(&t1,NULL,f1,(void*)&arg);
	pthread_create(&t2,NULL,f2,(void*)&arg);
	pthread_kill(t1,SIGUSR1);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

}
