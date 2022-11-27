#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<unistd.h>
struct msg{
    long type;
    char buf[2048];
};

int signalPid;

void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}

void get_pid1(int sig, siginfo_t *info, void *context){
    printf("P1 got signal from %d", info->si_pid);
    killpg(getpid(), SIGUSR2);
}
int main(){
    printf("P1 pid is %d\n", getpid());
    int next_pid, prev_pid;

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    
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
    
    printf("Receiving group id\n");
    //P1 catches the signal. Now P1 forms groupid
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
    pause();                           //wait for a signal
    printf("PID of signal sender = %d\n", signalPid);
    

    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = get_pid1;
    sigaction(SIGUSR2, &sa1, NULL);

    //setpgid(pid, pid_t pgid);
    struct msg msq4;
    msgrcv(msqid,&msq4,2048,18,0);

    i=0, pid=0;
    while(msq4.buf[i] != '\0'){
        pid = pid*10 + (int)msq4.buf[i] - 48;
        i++;
    }
    
    setpgid(pid, 0);

    struct msg msq5;
    msgrcv(msqid,&msq5,2048,18,0);

    i=0, pid=0;
    while(msq5.buf[i] != '\0'){
        pid = pid*10 + (int)msq5.buf[i] - 48;
        i++;
    }
    
    setpgid(pid, 0);
    
    struct msg msq6;
    msgrcv(msqid,&msq6,2048,18,0);

    i=0, pid=0;
    while(msq6.buf[i] != '\0'){
        pid = pid*10 + (int)msq6.buf[i] - 48;
        i++;
    }
    
    setpgid(pid, 0);
    
    struct msg msq7;
    msgrcv(msqid,&msq7,2048,18,0);

    i=0, pid=0;
    while(msq7.buf[i] != '\0'){
        pid = pid*10 + (int)msq7.buf[i] - 48;
        i++;
    }
    
    setpgid(pid, 0);

    struct msg msq1;
    msq1.type=2;
    sprintf(msq1.buf, "%d", getpid());
    printf("Message sending: %s\n", msq1.buf);
    msgsnd(msqid, &msq1, 2048, 0);

    struct msg msq2;
    msq1.type=3;
    sprintf(msq2.buf, "%d", getpid());
    printf("Message sending: %s\n", msq2.buf);
    msgsnd(msqid, &msq2, 2048, 0);

    struct msg msq3;
    msq1.type=4;
    sprintf(msq3.buf, "%d", getpid());
    printf("Message sending: %s\n", msq3.buf);
    msgsnd(msqid, &msq3, 2048, 0);


    //P1 sends a SIGUSR2 signal to the group
    killpg(getpid(), SIGUSR2);
    
    sleep(10);
}