#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h> 
using namespace std;
int main()
{
	int pp[2];
	char buffer1[1024];
	char buffer2[1024];
	pipe(pp);
	cin>>buffer1;
	write(pp[1],buffer1,1024);
	
	read(pp[0],buffer2,1024);
	cout<<buffer2<<endl;
	
}
