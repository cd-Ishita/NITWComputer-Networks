#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include<poll.h>
#include<pthread.h>
#include<semaphore.h>
#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold

sem_t mutex;
struct data{
    int sfd;
    int thread_num;
};

void *thread_service(void *args){

    struct data *arg = (struct data *)args;
    int sfd = arg->sfd;
    int thread_num = arg->thread_num;

    sem_wait(&mutex);

    printf("Hello");

    struct sockaddr_in my_addr;
    int len = sizeof(my_addr);

    struct sockaddr_storage client_address; // connector's address information
    socklen_t sin_size;
    int new_fd; //new connection
    char s[INET6_ADDRSTRLEN];

    sin_size = sizeof client_address;
    new_fd = accept(sfd, (struct sockaddr *)&client_address, &sin_size);
    if (new_fd == -1) {
        perror("accept");
    }

    char message_recv[2048] = {0};
    int bytes_recv = recv(new_fd, message_recv, 2047, 0);
    message_recv[bytes_recv] = '\0';
    printf("Message received is: %s\n", message_recv);

    if(thread_num == 1){
        //uppercase and send back
        for(int i=0;i<bytes_recv;i++){
            message_recv[i] = (char)((int)message_recv[i] + 33);
        }
        char *new_msg = message_recv;
        send(new_fd, new_msg, 2048, 0);
    }
    else{
        //lowercase and send back
        for(int i=0;i<bytes_recv;i++){
            message_recv[i] = (char)((int)message_recv[i] - 33);
        }
        char *new_msg = message_recv;
        send(new_fd, new_msg, 2048, 0);
    }
    sem_post(&mutex);
    pthread_exit(NULL);
    /*
    if (!fork()) { // this is the child process
        close(sfd); // child doesn't need the listener
        close(new_fd);
        exit(0);
    }
    close(new_fd);  // parent doesn't need this*/
}



int main(void){
    struct sockaddr_in hints;
    int port[3];
    port[0] = 3390;
    port[1] = 3490;
    port[2] = 3590;
    int sfd[3];

    for(int i=0;i<3;i++){
        sfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if(sfd[i] == -1){
            perror("Socket connection");
        }

        hints.sin_addr.s_addr = htonl(INADDR_ANY);
        hints.sin_family = AF_INET;

        hints.sin_port = htons(port[i]);

        int b_status = bind(sfd[i], (struct sockaddr *)&hints, sizeof(hints));
        if(b_status == -1){
            perror("Bind");
        }

        if(listen(sfd[i], 10) == -1){
            perror("Listen");
        }
    }

    struct pollfd pfds[3];

    for(int i=0;i<3;i++){
        pfds[i].fd = sfd[i];
        pfds[i].events = POLLIN;
    }

    int count = poll(pfds,3,0);
    
    sem_init(&mutex, 0, 1);
    pthread_t p[3];

    for(int i=0;i<3;i++){
        struct data args;
        args.sfd = sfd[i];
        args.thread_num = i;
        pthread_create(&p[i],NULL,thread_service,&args);
    }
    for(int i=0;i<3;i++){
        pthread_join(p[i],NULL) ;
    }


    return 0;
}