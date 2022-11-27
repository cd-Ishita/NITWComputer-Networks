#include<iostream>
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>      
#include <sys/wait.h>
#include<bits/stdc++.h>
#include<thread>
#define range 1024
using namespace std;

void readthread(int fd,char buff[range],int count)
{
	read(fd,buff,count);
}
void writethread(int fd,char buff[range],int count)
{
	write(fd,buff,count);
}
int main()
{
	pthread_t tid1,tid2;
	int A[2];
	int B[2];
	
	pipe(A);
	pipe(B);
	dup2(0,10);
    	dup2(1,11);
	dup2(A[0],0);
	dup2(B[1],1);
	int c=fork();
	if(c>0)
	{
		close(A[0]);
		close(B[1]);
		dup2(10,0);
        dup2(11,1);
		int cnt=2;
		while(cnt--)
		{
			char buffer1[1024]={'\0'};
			char buffer2[1024]={'\0'};
			cout<<"Parent :- Enter Data : ";
			cin>>buffer1;
			thread parent1(writethread,A[1],buffer1,range);
			parent1.join();
			thread parent2(readthread,B[0],buffer2,range);
			parent2.join();
			cout<<"Parent :- Data Read : "<<buffer2<<endl;
		}
		close(A[1]);
		close(B[0]);
		
		
	}
	else
	{
		execv("./p2",nullptr);
		
	}
	
	
	
}
