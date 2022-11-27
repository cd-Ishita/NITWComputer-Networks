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
    printf("C1 wants input: \n");
    char buf[2048];
    scanf("%s", buf);
    strcat("from c1 ", buf);
    printf("Writing %s to FIFO\n", buf);
    write(fd, buf, 2048);
    pthread_exit(NULL);
}
void receive_thread(int fd){
    char buf[2048];
    read(fd, buf, 2048);
    printf("C1 received ", buf);
    if(buf[0] == '1'){
        char buf1[2048];
        memcpy(buf1, buf+8, 2048);
        printf("C1 received ", buf1);
    }
    pthread_exit(NULL);
}
int main(){
    while(1){

        pthread_t send, receive;

        pthread_create(&send, NULL, &send_thread, NULL);
        sleep(10);
        pthread_create(&receive, NULL, &receive_thread, NULL);
        sleep(10);

    }
}