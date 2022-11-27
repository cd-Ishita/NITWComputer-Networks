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
    printf("P4 got signal from %d", info->si_pid);
    signalPid = info->si_pid;
}
int group_id;

void get_pid1(int sig, siginfo_t *info, void *context){
    printf("P4 got signal from %d", info->si_pid);
    killpg(group_id, SIGUSR2);
}
int main(){
    printf("P4 pid is %d\n", getpid());
    int next_pid, prev_pid;

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);

    //P4 gets excuted, and it sends a message containing its pid
    struct msg msq;
    msq.type=4;
        
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);

    //P4 catches the signal. P4 sends a message as
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
    pause();                           //wait for a signal
    printf("PID of signal sender = %d\n", signalPid);
    printf("Pid %d <- Pid %d\n", signalPid, getpid());
    prev_pid = signalPid;

    struct msg msq1;
    msq1.type = 18;    
    sprintf(msq1.buf, "%d", getpid());
    printf("Message sending: %s\n", msq1.buf);
    msgsnd(msqid, &msq1, 2048, 0);


    //P4 receives a message from msq and sends a SIGUSR1 to P1
    struct msg msq2;
    msgrcv(msqid,&msq2,2048,1,0);
    printf("Message content: %s\n", msq2.buf);
    
    int i=0, pid_2=0;
    while(msq2.buf[i] != '\0'){
        pid_2 = pid_2*10 + (int)msq2.buf[i] - 48;
        i++;
    }
    
    kill(next_pid, SIGUSR1);

    printf("Receiving group id\n");
    //Receives group id
    struct msg msq3;
    msgrcv(msqid,&msq3,2048,4,0);
    printf("Message content: %s\n", msq3.buf);
    
    i=0;
    while(msq3.buf[i] != '\0'){
        group_id = group_id*10 + (int)msq3.buf[i] - 48;
        i++;
    }

    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = get_pid1;
    sigaction(SIGUSR2, &sa1, NULL);


    return 0;
}