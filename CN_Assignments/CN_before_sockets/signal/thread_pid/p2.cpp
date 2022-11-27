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





int len=1024;
int id=1;
pthread_t t1,t2;
void handler1(int count) {
	cout<<"signal caught in thread1 \n";
  sleep(1);
pthread_kill(t1,SIGUSR2);
}
void handler2(int count) {
	cout<<"signal caught in thread2 \n";
  sleep(1);
pthread_kill(t1,SIGUSR1);
}


void * readfunc(void *arg){
	signal(SIGUSR1,handler1);
while(1){
}
}


void * writefunc(void *arg){
  signal(SIGUSR2,handler2);
  while(1){
      }
}




int main(){
  int t,pid,wpid,status=0;
  cout<<"started"<<endl;
int arg1;
pthread_create(&t1,NULL,readfunc,( void *)&arg1);
pthread_create(&t2,NULL,writefunc,( void *)&arg1);
pthread_kill(t1,SIGUSR1);
pthread_join(t1,NULL);
pthread_join(t2,NULL);





return 0;
}
