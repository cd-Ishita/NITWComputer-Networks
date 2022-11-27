#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<poll.h>
int main(){

    mkfifo("FIFO1", 0666);
    mkfifo("FIFO2", 0666);
    mkfifo("FIFO3", 0666);

/*
    int c = fork();
    if(c > 0){

        //int c1 = fork();
        if(c1 > 0){
            int c2 = fork();
            
            if(c2 > 0){*/
                int fd1 = open("FIFO1", O_RDONLY);
                int fd2 = open("FIFO2", O_RDONLY);
                int fd3 = open("FIFO3", O_RDONLY);

                struct pollfd fds[3];
                int timeout;
                
                printf("Hi");
                while(1){
                    fds[0].fd = fd1;
                    fds[0].events = 0; //just to clear any previous events
                    fds[0].events |= POLLIN;
                    fds[0].revents = 0;

                    fds[1].fd = fd2;
                    fds[1].events = 0; //just to clear any previous events
                    fds[1].events |= POLLIN;
                    fds[1].revents = 0;

                    fds[2].fd = fd3;
                    fds[2].events = 0; //just to clear any previous events
                    fds[2].events |= POLLIN;
                    fds[2].revents = 0;

                    timeout = 5000; //in milliseconds
                    int pret = poll(fds, 3, timeout);

                    if(pret == 0){
                        printf("No input so timeout\n");
                    }
                    else if(pret > 0){
                        printf("Hello input is given\n");
                        if(fds[0].revents & POLLIN){
                            char buf[2048];
                            int i = read(fds[0].fd, buf, 2048);
                            printf("buf = %s", buf);
                            if(!i){
                                printf("We got input this: %s\n", buf);
                            }

                            fds[0].revents = 0;
                        }
                        if(fds[1].revents & POLLIN){
                            char buf[2048];
                            int i = read(fds[1].fd, buf, 2048);
                            printf("buf = %s", buf);
                            if(!i){
                                printf("We got input this: %s\n", buf);
                            }
                            fds[1].revents = 0;   
                        }
                        if(fds[2].revents & POLLIN){
                            char buf[2048];
                            int i = read(fds[2].fd, buf, 2048);
                            printf("buf = %s", buf);
                            if(!i){
                                printf("We got input this: %s\n", buf);
                            }

                            fds[2].revents = 0;
                        }   
                    }
                }
            /*}
            else{
                char *args[] = {"./p2", NULL};
                execv(args[0], args);
            }
        }   
        else{
            char *args[] = {"./p3", NULL};
            execv(args[0], args);
        }
    }
    else{
        char *args[] = {"./p4", NULL};
        execv(args[0], args);
    }

    int fd= 0;*/
}