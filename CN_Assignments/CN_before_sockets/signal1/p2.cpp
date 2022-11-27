#include <time.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/select.h>
#include <sys/un.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <netinet/ether.h>
#include <netinet/udp.h>
#include <semaphore.h>
#include <bits/stdc++.h>
using namespace std;

int p1pid;
int *shm1, *shm2;
key_t key1, key2;
int shmid1, shmid2;

static void handler2(int sig)
{
    cout << "PROCESS 2 RECEIVED SIGNAL\n";

    int ppid = *shm1;

    cout << "Process 2 Read: " << ppid << "\n";
    *shm2 = ppid;
    cout << "WRITTEN BY PROCESS 2\n";
    sleep(1);
    kill(p1pid, SIGUSR2);
}

int main()
{
    cout << "Process P2: Execution Started\n";
    signal(SIGUSR1, handler2);

    key1 = ftok(".", 'x');
    shmid1 = shmget(key1, sizeof(int), 0666);
    shm1 = (int *)shmat(shmid1, NULL, 0);
    key2 = ftok(".", 'y');
    shmid2 = shmget(key2, sizeof(int), 0666);
    shm2 = (int *)shmat(shmid2, NULL, 0);

    int s = *shm1;
    cout << "PID of P1 read = " << s << endl;
    p1pid = s;

    int pid = getpid();

    cout << pid << " : P2 ABOUT TO WRITE THIS\n";

    *shm2 = pid;
    cout << "PROCESS P2 Send SIGNAL TO P1\n";
    kill(p1pid, SIGUSR2);
    while (1)
    {
        cout << "Process 2 is waiting for signal\n";
        sleep(1);
    }
}
