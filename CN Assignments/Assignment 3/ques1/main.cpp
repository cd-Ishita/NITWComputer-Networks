#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
using namespace std ;
int main(){

	int pfd1[2], pfd2[2];
	pipe(pfd1);
	pipe(pfd2);

	int c = fork();

	if(c > 0){
		close(pfd1[0]);
		close(pfd2[1]);

		cout<<"This is the parent"<<endl ;

		//char buf1[2048] = "keyboard input to parent" ;
		char buf1[2048];
		cout<<"Parent wants input"<<endl;
		cin>>buf1;
		write(pfd1[1], buf1, 2048);

		wait(NULL);

		char buf2[2048];
		read(pfd2[0], buf2, 2048);
		cout<<"Parent read "<<buf2<<" from the child"<<endl;
	}
	else{
		close(pfd1[1]);
		close(pfd2[0]);

		cout<<"This is the child "<<endl;
		
        char buf1[2048];
        read(pfd1[0], buf1, 2048);
        cout<<"Child read "<<buf1<<" from parent"<<endl;

        //char buf2[2048] = "Parent Hi";
        char buf2[2048];
        cout<<"Child wants input";
        cin>>buf2;
        cout<<"Child is sending "<<buf2<<" to parent"<<endl;

        write(pfd2[1], buf2, 2048);

	}
	return 0;
}