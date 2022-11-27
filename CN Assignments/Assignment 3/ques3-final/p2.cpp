#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
using namespace std ;
int main(){
	int pfd3[2], pfd4[2];
	pipe(pfd3);
	pipe(pfd4);

	int c = fork();

	if(c > 0){
		
		close(pfd3[0]);
		close(pfd4[1]);

		char buf1[2048];
		read(0, buf1, 2048);

		cout<<"Child read "<<buf1<<" from the parent and forwards to grandchild"<<endl; ;

		write(pfd3[1], buf1, 2048);

		wait(NULL);

		char buf2[2048];
		read(pfd4[0], buf2, 2048);

		cout<<"Child read "<<buf2<<" from the grandchild and forwards to parent"<<endl;

		write(1, buf2, 2048);

	}
	else{
		close(pfd3[1]);
		close(pfd4[0]);

		dup2(pfd3[0], 0);
		dup2(pfd4[1], 1);

		close(pfd3[0]);
		close(pfd4[1]);

		char *args[] = {(char *)("./p3"), NULL};
		execv(args[0], args);
	}
	
	return 0;
}