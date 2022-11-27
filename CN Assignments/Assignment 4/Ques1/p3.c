//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include <semaphore.h>
void capitalise(char buf[]){
    char a = buf[0];
    int x = (int)a;
    x = x - 32;
    buf[0] = (char)x;

/*
    int i = strlen(buf);
    buf[i] = "f";
    buf[i+1] = "r";
    buf[i+2] = '\0';
*/}
int main(){

    char * green = "green";

    int ffd = mkfifo(green, 0666);

    int fd1 = open(ffd, O_WRONLY);

    //while(1){
        char buf[2048];
        scanf("%s", buf);
        printf("p3 got %s\n", buf);

        //close(1);
        capitalise(buf);
        printf("Capitalised %s\n", buf);
        strcat(buf, " from p3");
        printf("After adding %s\n", buf);
        write(fd1, *buf, 2048);
        close(fd1);
        unlink(ffd);
}