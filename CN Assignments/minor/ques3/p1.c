#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(){
    sleep(5);
    char buffer[2048] = "Hello from Process 1";
    //write(1, buffer, 2048);
}