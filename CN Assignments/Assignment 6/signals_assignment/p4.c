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
int signalPid;

void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}
int main(){
    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
    int next_pid, prev_pid;
    
    //P4 gets executed, and it sends a message containing its pid as p3pid
    struct msg msq;
    msq.type=4;
        
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);
    //msgctl(msqid, IPC_RMID, NULL) ;


    //P4 catches the signal and it finds out who has sent that signal to it
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
    pause();                           //wait for a signal
    printf("PID of signal sender = %d\n", signalPid);
    printf("Pid %d <- Pid %d\n", signalPid, getpid());
    prev_pid = signalPid;


    //P4 receives a message from msq (there is one message left in msq)
    struct msg msq1;
    msgrcv(msqid,&msq1,2048,1,0);
    printf("Message content: %s\n", msq1.buf);
    int i=0, pid_1=0;
    while(msq1.buf[i] != '\0'){
        pid_1 = pid_1*10 + (int)msq1.buf[i] - 48;
        i++;
    }
    printf("Pid %d -> Pid %d", getpid(), pid_1);
    next_pid = pid_1;

    //sends a SIGUSR1 to P1
    kill(next_pid, SIGUSR1);

    printf("Prev pid %d and next pid %d\n", prev_pid, next_pid);

    for(int i=0;i<3;i++){
        printf("Signalling pid %d from P4\n", next_pid);
        kill(next_pid, SIGUSR1);
    }

    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = get_pid;
    sigaction(SIGUSR2, &sa1, NULL);
    for(int i=0;i<3;i++){
        printf("Signalling pid %d from P4\n", prev_pid);
        kill(prev_pid, SIGUSR2);
    }
    /*
    signal(SIGUSR1, SIG_DFL);
    for(int i=0;i<3;i++){
        kill(next_pid, SIGUSR1);
        struct sigaction sa1;
        sa1.sa_flags = SA_SIGINFO;
        sa1.sa_sigaction = get_pid;
        sigaction(SIGUSR1, &sa1, NULL);
        pause();                           //wait for a signal
        printf("Pid %d -> ", getpid());
    }
    /*
    signal(SIGUSR2, SIG_DFL);
    for(int i=0;i<3;i++){
        kill(prev_pid, SIGUSR2);
        struct sigaction sa1;
        sa1.sa_flags = SA_SIGINFO;
        sa1.sa_sigaction = get_pid;
        sigaction(SIGUSR2, &sa1, NULL);
        pause();                           //wait for a signal
        printf("Pid %d -> ", getpid());
    }
    */
    return 0 ;
 }