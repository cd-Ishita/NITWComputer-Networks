#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
using namespace std;
int main()
{
	char buffer[1024];
	read(0,buffer,1024);
	FILE* fd=popen("./p3","w");
	cout<<"p2: "<<buffer<<endl;
	write(fileno(fd),buffer,1024);
	
	
	
}
