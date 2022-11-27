#include<iostream>
#include<unistd.h>
#include <stdio.h> 
#include<cstring>
using namespace std;
int main()
{
	char buffer2[1024];
	read(0,buffer2,1024);
	cout<<"p2: "<<buffer2<<endl;
	
	
	
}
