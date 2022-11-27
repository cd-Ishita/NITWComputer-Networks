#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<poll.h>
#include<pthread.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <sys/wait.h>
#include<arpa/inet.h>
#include<poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
struct thread_args{
    int id;
    char *buffer;
    int sfd;
};

// Thread function, every thread for every client uses this
void* func(void *args){
    struct thread_args *arg = (struct thread_args*)args;
    int server_num = arg->id;
    char* buffer = arg->buffer;
    int nsfd = arg->sfd;

    write(nsfd, buffer, 2048);
    printf("The client request has been sent to server %d", server_num);

}

int open_socket(int port_num){
    int opt = 1, sfd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Creating socket file descriptor
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port 8080
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_num);
    
    // Forcefully attaching socket to the port 8080
    if (bind(sfd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sfd, 5) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("sfd is listening for connections\n");

    int nsfd = accept(sfd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
    printf("Facilitator is connected to server\n");
    return nsfd;
}
int open_server(char buffer[2048]){
    char port[2048];
    int j=0, i=1, port_num = 0;
    while(buffer[i] != ','){
        port_num = port_num*10 + (int)buffer[i] - 48;
        port[j] = buffer[i];
        i++;
        j++;
    }
    i++;
    i++;

    char path[2048];
    j=0;
    while(buffer[i] != ')'){
        path[j] = buffer[i];
        i++;
        j++;
    }

    int pfd[2];
    pipe(pfd);

    int c = fork();
    if(c > 0){
        close(pfd[0]);
        //port number sent to server file
        write(pfd[0], port, 2048);
        close(pfd[0]);
        int sfd = open_socket(port_num);
        return sfd;
    }
    else{
        close(pfd[0]);
        dup2(pfd[1], 1);
        close(pfd[1]);

        char *args[] = {path, NULL};
        execv(args[0], args);
    }
}
int main(){
    int num = 1;
    int sfd[num];
    char c = 'a';
    for(int i=0;i<num;i++){
        printf("Enter (port_number,/path): ");
        char buffer[2048];
        fgets(buffer, 2048, stdin);
        
        sfd[i] = open_server(buffer);
    }
    
    int main_sfd;
    struct sockaddr_in servaddr, cliaddr;
       
    // Creating socket file descriptor
    if ( (main_sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    /*if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }*/
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
       
    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(6060);
       
    // Bind the socket with the server address
    if ( bind(main_sfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("main connectionless sfd is ready for client connections\n");

    pthread_t threads[num];
    int free_threads = num;
    while(1){
        char buffer[2048];
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        recvfrom(main_sfd, (char *)buffer, 2048, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);

        int server_no = (int)buffer[0] - 48;
        if(server_no > num){
            printf("This service does not exist\n");
            continue;
        }

        recvfrom(main_sfd, (char *)buffer, 2048, MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
        printf("Server received: %s", buffer);
        struct thread_args arg;
        arg.id = server_no;
        arg.buffer = buffer;
        arg.sfd = sfd[server_no];
        pthread_create(&threads[server_no],NULL,func,(void *)&arg);
    }
}