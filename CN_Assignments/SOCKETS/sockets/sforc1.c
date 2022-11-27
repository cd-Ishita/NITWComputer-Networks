#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>  
int main()
{  
    char buffer[1024]; 
    int valread;
    valread = read( 0 , buffer, 1024); 
    printf("%s in 1st child \n",buffer); 
    for(int i=0;i<strlen(buffer);i++)
    {
    	if(buffer[i]>='a' && buffer[i]<='z')
    	buffer[i]=buffer[i]-'a'+'A';
    }
    strcat(buffer,"-> captilised it");
    send(0 ,buffer, strlen(buffer) , 0 ); 
    printf("Hello message sent from 1 st child server \n");
    return 0;
} 
