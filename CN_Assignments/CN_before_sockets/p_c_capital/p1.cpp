#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
using namespace std;
int main()
{
	int A[2];
	int B[2];
	char buffer1[1024];
	char buffer2[1024]={'\0'};
	pipe(A);
	pipe(B);
	int c=fork();
	if(c>0)
	{
		close(A[0]);
		close(B[1]);
		int cnt=2;
		while(cnt--)
		{
			cin>>buffer1;
			write(A[1],buffer1,1024);
			read(B[0],buffer2,1024);
			cout<<buffer2<<endl;	
		}
		close(A[1]);
		close(B[0]);
		
		
	}
	else
	{
		close(A[1]);
		close(B[0]);
		while(1){
		
		read(A[0],buffer1,1024);
		int i;
		memset(buffer2,0,sizeof(buffer2));
		for( i=0;i<1024;i++)
		{
			if(buffer1[i]!='\0')
			{
//				cout<<"1"<<endl;
				buffer2[i]=buffer1[i]-32;
				
			}
			else
			break;
		}
	//	cout<<buffer2<<endl;
		write(B[1],buffer2,1024);
		}
		close(A[0]);
		close(B[1]);
		
	}
	
	
	
}
