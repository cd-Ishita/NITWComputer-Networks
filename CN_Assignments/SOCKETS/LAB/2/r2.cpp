#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>      
#include <sys/wait.h>
#include<bits/stdc++.h>
using namespace std;
#define e 1024
int main(){
    
    int k=2;
    int p=8080;
    for(int i=1;i<100;i++)
    {
    
    	if(i%(10+2*k-2)==0)
    	cout<<p<<" "<<endl; 
    	if(i%(10+2*k-1)==0)
    	cout<<"/d"<<" ";
    	cout<<"hi"<<k*100+i<<endl;
    	sleep(5);
    	
    
    }
}
