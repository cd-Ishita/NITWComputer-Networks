#include<bits/stdc++.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
using namespace std;

int main()
{
	cout<<"p1 is executing\n";
	int c=0;
	c=fork();
	if(c>0)
	{
		cout<<"p1 : my child p2 is starting \n";
		char **s;
		execv("./p2",s);
		cout<<"p1 : my child p2 is exited \n";
	}
	else
	{
		int w=10;
		wait(&w);
		
		cout<<"p1 : my child p3 is starting \n";
		char **s;
		execv("./p3",s);
		cout<<"p1 : my child p3 is exited \n";
	}
}
