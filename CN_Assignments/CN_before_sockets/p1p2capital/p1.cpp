#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
#include<fcntl.h>
using namespace std;
int main()
{
	int A[2];
	int B[2];
	char buffer1[1024];
	char buffer2[1024];
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
			cout<<"input in parent :";
			cin>>buffer1;
			write(A[1],buffer1,1024);
			read(B[0],buffer2,1024);
			cout<<"output in parent: "<<buffer2<<endl;	
		}
		
		close(A[1]);
		close(B[0]);	
	}
	else
	{
		execv("./p2",nullptr);
		
	}
	
	
	
}
