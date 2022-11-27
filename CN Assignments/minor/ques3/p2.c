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
    mkfifo("green", 0666);

    int fd = open("green", O_WRONLY);
    //sleep(5);
    char buf[2048];
    printf("P2 asks input: ");
    scanf("%s", buf);
    write(fd, buf, 2048);
    printf("P2 sent data to Server");
}