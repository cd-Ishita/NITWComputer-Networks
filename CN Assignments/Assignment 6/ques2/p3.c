#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<string.h>
#include <unistd.h>
struct msg{
    long type ;
    char buf[2048];
};
int signalPid, group_id;

void get_pid(int sig, siginfo_t *info, void *context){
    printf("P3 got signal from %d", info->si_pid);
    signalPid = info->si_pid;
}

void get_pid1(int sig, siginfo_t *info, void *context){
    printf("P3 got signal from %d", info->si_pid);
    killpg(group_id, SIGUSR2);
}
int main(){
    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
    int next_pid, prev_pid;
    //P3 gets executed, and it sends a message containing its pid as p3pid
    struct msg msq;
    msq.type=3;
        
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);
    //msg(msqid, IPC_RMID, NULL) ;


    //P3 catches the signal. P3 sends a message as : type = 18, content = p3pid
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
    pause();                           //wait for a signal
    printf("PID of signal sender = %d\n", signalPid);
    printf("Pid %d <- Pid %d\n", signalPid, getpid());
    prev_pid = signalPid;


    struct msg msq1;
    msq.type=18;   
    sprintf(msq1.buf, "%d", getpid());
    printf("Message sending: %s\n", msq1.buf);
    msgsnd(msqid,&msq1,2048,0);


    //P3 receives the message of type = 4
    struct msg msq2;
    msgrcv(msqid,&msq2,2048,4,0);
    printf("Message content: %s\n", msq2.buf);
    
    int i=0, pid_3=0;
    while(msq2.buf[i] != '\0'){
        pid_3 = pid_3*10 + (int)msq2.buf[i] - 48;
        i++;
    }
    printf("Pid %d -> Pid %d", getpid(), pid_3);
    next_pid = pid_3;

    //P3 sends a signal SIGUSR1
    kill(next_pid, SIGUSR1);

    printf("Receiving group id\n");
    //Receives group id
    struct msg msq3;
    msgrcv(msqid,&msq3,2048,3,0);
    printf("Message content: %s\n", msq3.buf);
    
    i=0;
    while(msq3.buf[i] != '\0'){
        group_id = group_id*10 + (int)msq3.buf[i] - 48;
        i++;
    }

    //
    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = get_pid1;
    sigaction(SIGUSR2, &sa1, NULL);
    
}