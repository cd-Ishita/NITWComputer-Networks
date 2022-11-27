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

	char buf1[2048];
	read(0, buf1, 2048);
	
	cout<<"Grandchild read "<<buf1<<" from pipe"<<endl;

	char buf2[2048] = "Hello from grandchild";
	//char buf2[2048];

	//cout<<"Grandchild wants input"<<endl;
	//cin>>buf2;
	write(1, buf2, 2048);

	return 0;
}