//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
#include <semaphore.h>
using namespace std;

sem_t mutex;

int main(){

	sem_init(&mutex, 0, 1);

	int pfd1[2], pfd2[2];
	pipe(pfd1);
	pipe(pfd2);

	int c;
	cout<<"This is the main process"<<endl;

	c = fork();
	if(c > 0){
		sem_wait(&mutex);

		cout<<"This is the parent process"<<endl;
		close(pfd1[1]);
		close(pfd2[0]);

		//char buf1[2048] = "Hello From Parent";
		char buf1[2048];
		cout<<"Parent wants input: ";
		cin>>buf1;
		write(pfd2[1], buf1, 2048);

		cout<<"Parent is sending "<<buf1<<" to pipe\n";
		sem_post(&mutex);
		wait(NULL);
		sem_wait(&mutex);
		char buf2[2048];
		read(pfd1[0], buf2, 2048);

		cout<<"Parent read "<<buf2<<" from pipe\n";
		sem_post(&mutex);
	}
	else{
		close(pfd1[0]);
		close(pfd2[1]);

		dup2(pfd2[0], 0);
		dup2(pfd1[1], 1);

		close(pfd2[0]);
		close(pfd1[1]);

		char *args[] = {(char *)("./p1"), NULL};
		execv(args[0], args);
	}
	//wait(NULL);
	cout<<"Child process has ended, only parent is here\n";
}