//compile with -lpthread -lrt
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
void send_thread(int fd){
    printf("C2 wants input: \n");
    char buf[2048];
    scanf("%s", buf);
    strcat("from c2 ", buf);
    printf("C2 is writing %s to FIFO", buf);
    write(fd, buf, 2048);
    pthread_exit(NULL);
}
void receive_thread(int fd){
    char buf[2048];
    read(fd, buf, 2048);

    printf("C2 received ", buf);
    if(buf[5] == '2'){
        char buf1[2048];
        memcpy(buf1, buf+6, 2048);
        printf("C2 received ", buf1);
    }
    pthread_exit(NULL);
}
int main(){
    while(1){

        pthread_t send, receive;
        sleep(10);
        pthread_create(&send, NULL, &send_thread, NULL);
        sleep(10);
        pthread_create(&receive, NULL, &receive_thread, NULL);
    }
}