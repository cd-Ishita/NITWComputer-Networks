#include<bits/stdc++.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h>  
using namespace std; 

int main() 
{ 
	key_t key = ftok("shmfile",65); 
	int shmid = shmget(key,1024,0666|IPC_CREAT); 
	char * s = (char*) shmat(shmid,(void*)0,0); 
	while(s!="#")
	{
		cout<<"reader: "<<s<<endl;
		cout<<"writers turn "; 
		cin.getline(s,1024,'\n');
	}
	
	shmctl(shmid,IPC_RMID,NULL); 
	
	return 0; 
} 

