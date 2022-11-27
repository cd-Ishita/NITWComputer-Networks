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
    //int sz ;
    char buf[2048];
};

int signalPid;

void get_pid(int sig, siginfo_t *info, void *context){
    signalPid = info->si_pid;
}

int main(){
    int next_pid, prev_pid;

    key_t key=ftok("message1",65); 
    int msqid=msgget(key,0666 | IPC_CREAT);
    

    //P2 gets executed first, P2 sends a message containing its pid as : p2pid with type value 2
    struct msg msq;
    msq.type=2;
    sprintf(msq.buf, "%d", getpid());
    printf("Message sending: %s\n", msq.buf);
    msgsnd(msqid,&msq,2048,0);

    //msgctl(msqid, IPC_RMID, NULL) ;

    //P2 catches the signal and it finds out, who has sent that signal to it
    //taking SIGUSR1 from P1 and finding pid
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = get_pid;
    sigaction(SIGUSR1, &sa, NULL);
    pause();                           //wait for a signal
    printf("PID of signal sender = %d\n", signalPid);
    printf("Pid %d <- Pid %d\n", signalPid, getpid());
    prev_pid = signalPid;

    printf("Enter char to continue: \n");
    char c;
    scanf("%c", &c);

    //P2 receives the message from msq and it notes that its next process is P3
    struct msg msq3;
    msgrcv(msqid,&msq3,2048,3,0);
    printf("Message content: %s\n", msq3.buf);
    
    int i=0, pid_3=0;
    while(msq3.buf[i] != '\0'){
        pid_3 = pid_3*10 + (int)msq3.buf[i] - 48;
        i++;
    }
    printf("Pid %d -> Pid %d", getpid(), pid_3);
    next_pid = pid_3;
    

    //P2 sends a message containing P1's pid as type = 1
    struct msg msq4;
    msq4.type=1;
        
    
    sprintf(msq4.buf, "%d", prev_pid);
    printf("Message sending: %s\n", msq4.buf);
    msgsnd(msqid,&msq4,2048,0);

    //P2 sends a signal SIGUSR1 to P3
    kill(next_pid, SIGUSR1);
    
    //msgctl(msqid, IPC_STAT, NULL);
	//msgctl(msqid, IPC_RMID, NULL);

    printf("Prev pid %d and next pid %d\n", prev_pid, next_pid);

    for(int i=0;i<3;i++){
        printf("Signalling pid %d from P2\n", next_pid);
        kill(next_pid, SIGUSR1);
    }

    struct sigaction sa1;
    sa1.sa_flags = SA_SIGINFO;
    sa1.sa_sigaction = get_pid;
    sigaction(SIGUSR2, &sa1, NULL);
    for(int i=0;i<3;i++){
        printf("Signalling pid %d from P2\n", prev_pid);
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