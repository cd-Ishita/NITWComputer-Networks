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
    mkfifo("FIFO2", 0666);

    int fd = open("FIFO2", O_WRONLY);
    sleep(2);
    char buf[2048];
    printf("Process 3 asks input: ");
    scanf("%s", buf);
    write(fd, buf, 2048);


    close(fd);
    unlink("FIFO2");
}