#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<bits/stdc++.h>
#include <fstream>
using namespace std;


static struct sigaction siga;
key_t key=ftok(".",65);

struct shmem
{
	int i;
};
int k,shmid;
struct shmem *z;

/*

static void multi_handler(int sig, siginfo_t *siginfo, void *context) 
{
    // get pid of sender,
    pid_t sender_pid = siginfo->si_pid;
    cout<<"signal from"<<sender_pid<<endl;
    sleep(1);
   kill((pid_t)k,SIGUSR1);
}
*/


int main()
{
	cout<<"process of p11:"<<getpid()<<endl;
//	siga.sa_sigaction = *multi_handler;
  //  	siga.sa_flags |= SA_SIGINFO;
  //  	sigaction(SIGUSR2, &siga, NULL);
    	shmid = shmget(key,sizeof(shmem),IPC_CREAT|0660);
	z = (struct shmem*)shmat(shmid,NULL,0);
	
    	k=z->i;
   // 	z->i=getpid();
	ofstream fout;		
 	fout.open("config.txt");
 	int n;
 	cin>>n;
 	for(int i=0;i<n;i++)
 	{
 		string s=to_string(i+1);
 		s="./s"+s;
 		fout<<8080+100*i<<"\t"<<"tcp\t"<<10<<"\t"<<s<<endl;
 	
 	
 	}
 //	 FILE *fp=popen("ps ax | grep s","r");
 int m;
 	cin>>m;
 	for(int i=n;i<n+m;i++)
 	{
 		string s=to_string(i+1);
 		s="./s"+s;
 		fout<<8080+100*i<<"\t"<<"tcp\t"<<10<<"\t"<<s<<endl;
 	
 	
 	}
 	fout.close();

    	kill((pid_t)k,SIGUSR1);
    	 cout<<"hi"<<endl;
    	
    	
  //	cout<<"p1="<<z->i<<endl;
  	while(1);



}
