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
		system("g++ p2.cpp -o p2");
		system("./p2");
		cout<<"p1 : my child p2 is exited \n";
	}
	else
	{
		sleep(10);
		cout<<"p1 : my child p3 is starting \n";
		system("g++ p3.cpp -o p3");
		system("./p3");
		
		cout<<"p1 : my child p3 is exited \n";
	}
}
