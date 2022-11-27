#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>
struct msg{
    long type;
    //int sz ;
    char buf[2048];
};

int signalPid1, signalPid2;

void get_pid1(int sig, siginfo_t *info, void *context){
    signalPid1 = info->si_pid;
}
void get_pid2(int sig, siginfo_t *info, void *context){
    signalPid2 = info->si_pid;
}
int main(){
    printf("P1 pid is %d\n", getpid());
    int next_pid, prev_pid;

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = get_pid2;
    sigaction(SIGUSR2, &sa1, NULL);

    struct msg msq;

    //P1 gets executed, it receives message and finds p2pid from message and sends SIGUSR1 to P2
    msgrcv(msqid,&msq,2048,2,0);

    printf("Message content: %s\n", msq.buf);
    
    //find pid
    int i=0, pid=0;
    while(msq.buf[i] != '\0'){
        pid = pid*10 + (int)msq.buf[i] - 48;
        i++;
    }
    
    printf("Pid %d -> Pid %d\n", getpid(), pid);
    next_pid = pid;

    kill(pid, SIGUSR1);
    


    //P1 catches the signal and it finds out, who has sent that signal to it
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid1;
    sigaction(SIGUSR1, &sa, NULL);
    pause();                           //wait for a signal
    printf("PID of signal sender = %d\n", signalPid1);
    printf("Pid %d <- Pid %d\n", signalPid1, getpid());
    prev_pid = signalPid1;

    printf("Prev pid %d and next pid %d\n", prev_pid, next_pid);
    

    //signal(SIGUSR1, SIG_DFL);
    for(int i=0;i<3;i++){
        printf("Signalling pid %d from P1\n", next_pid);
        kill(next_pid, SIGUSR1);
    }

    for(int i=0;i<3;i++){
        printf("Signalling pid %d from P1\n", prev_pid);
        kill(prev_pid, SIGUSR2);
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
    //msgctl(msqid, IPC_STAT, NULL);
	//msgctl(msqid, IPC_RMID, NULL);
    return 0 ;
 }