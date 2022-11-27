#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
using namespace std;
int main()
{
	char buffer[1024];
	read(0,buffer,1024);
	cout<<"p4: "<<buffer<<endl;
	
	
	
}
