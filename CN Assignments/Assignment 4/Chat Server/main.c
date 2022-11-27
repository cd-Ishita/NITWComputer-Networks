//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
void read_thread(int fd, int fd1){
    char buf[2048];
    fscanf(fd, buf, 2048);
    if(buf[0] == '\0'){
        return;
    }
    if(buf[7] == '1'){
        char buf1[2048];
        memcpy(buf1, buf+8, 2048);
        strcat("to c1 ", buf1);
        write(fd1, buf1, 2048);
    }
    else if(buf[7] == '2'){
        char buf1[2048];
        memcpy(buf1, buf+8, 2048);
        strcat("to c2 ", buf1);
        write(fd1, buf1, 2048);
    }
    else if(buf[7] == '3'){
        char buf1[2048];
        memcpy(buf1, buf+8, 2048);
        strcat("to c3 ", buf1);
        write(fd1, buf1, 2048);
    }
}
int main(){
    int ffd = mkfifo("send", 0666);
    int ffd1 = mkfifo("receive", 0666);

    while(1){
        int fd1 = open(ffd, O_WRONLY);
        int fd2 = open(ffd1, O_RDONLY);

        read_thread(fd2, fd1);

        close(fd1);
        close(fd2);
        sleep(5);
    }
}