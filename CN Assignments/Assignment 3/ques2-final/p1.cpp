#include<stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
using namespace std;

sem_t mutex;

int main(){
	sem_init(&mutex, 0, 1);

	sem_wait(&mutex);
	char buf2[2048];
	read(0, buf2, 2048);
	cout<<"Child has read "<<buf2<<" from pipe"<<endl;
	sem_post(&mutex);

	sem_wait(&mutex);
	char buf1[2048] = "hello from child";
	write(1, buf1, 2048);
	cout<<"Child is writing "<<buf1<<" to pipe"<<endl;
	sem_post(&mutex);

	sem_destroy(&mutex);
	return 0 ;
}