//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>

void read_thread(FILE* fd){ //popen
    char buf[2048];
    fscanf(stdin, "%[^\n]s", buf);
    printf("Main is printing %s to p2\n", buf);
    fprintf(fd, buf);
}

void write_thread(FILE* fd){
    char buf[2048];
    fscanf(fd, buf, 2048);
    printf("Thread read: %s",buf);
}
int main(){
    FILE* fd = popen("./p2", "w");

    //char *myfifo = "/tmp/myfifo";
    
    char * green = "green";
    
    int ffd = mkfifo(green, 0666);

    /*
    while(fgetc(fd) != '\n'){
        char buf[2048];
        char c = fgetc(stdin);
        printf("Main read: %s", c);
        fprintf(fd, c);
        sleep(1);
    }
    printf("OUT"); */
    read_thread(fd);

    //close(ffd);
    pclose(fd);
    sleep(1);
    //printf("OUT");
    int fd1 = open(ffd, O_RDONLY);
    

    char buf1[2048];
    read(fd1, buf1, 10);
    
    //printf("blah %d%d%d%d%d", buf1[0], buf1[1], buf1[2], buf1[3], buf1[4]);
    printf("Main Program got %s from FIFO\n", buf1);
    close(fd1);

    
    //unlink(ffd);
}