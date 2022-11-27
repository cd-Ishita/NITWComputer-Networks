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
#include<pthread.h>  
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/select.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include<stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h> 
#include <sys/socket.h>
#include <sys/stat.h> 
#include <sys/select.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/wait.h>

struct msg{
    long type ;
    char buf[2048];
};

struct thread_args{
    int id;
    int msqid;
};
int scores[10];

void *func(void *args){
    struct thread_args* arg = (struct thread_args*)args;
    int id = arg->id;
    int msqid = arg->msqid;
    int score = 0;

    sleep(10);
    while(score != 3){
        struct msg msq;
    
        msgrcv(msqid, &msq, 2048, id, 0);
        scores[id] = atoi(msq.buf);
        score++;
    }
}

int threshold_candidates = 10;
int main(int argc, char *argv[]) {
	
    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
    pthread_t thread[10];


    for(int i=0;i<threshold_candidates;i++){
        struct thread_args arg;
        arg.id = i;
        arg.msqid = msqid;
        pthread_create(&thread[i], NULL, func, &arg);
    }
    
    msgctl(msqid, IPC_RMID, NULL) ;
}