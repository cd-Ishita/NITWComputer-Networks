#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
using namespace std;
int main()
{
	char buffer1[1024];
	FILE* fd=popen("./p2","w");
	cout<<"read in p1:";
	cin>>buffer1;
	write(fileno(fd),buffer1,1024);
		
}
