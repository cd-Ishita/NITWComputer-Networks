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

int main(){
    FILE* fd = popen("./p3", "w");
    dup2(fd, 1);

    
    //while(1){
        char buf[2048];
        scanf("%s", buf);
        printf("p2 got %s \n", buf);
        fprintf(fd, buf);
    //}

    pclose(fd);
}