#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>
int main(){
    // ftok to generate unique key
    key_t key1 = ftok("x",65);
  
    // shmget returns an identifier in shmid
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    char *data1 = (char*) shmat(shmid1,(void*)0,0);
	int pid = atoi(data1);
    
    while(1){
        sleep(5);
        kill(pid, SIGUSR1);
    }
}