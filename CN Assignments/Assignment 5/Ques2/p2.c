//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    mkfifo("FIFO1", 0666);

    int fd = open("FIFO1", O_WRONLY);
    sleep(2);
    char buf[2048];
    printf("Process 2 asks input: ");
    scanf("%s", buf);
    write(fd, buf, 2048);


    close(fd);
    unlink("FIFO1");
}