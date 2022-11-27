#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include<semaphore.h>
#include <sys/msg.h>
#include <math.h>
#include <sys/shm.h>
//compile with -lpthread -lrt
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include<pthread.h>
struct thread_args{
    int sfd;
    struct sockaddr_in address;
};
int client_sfd[10];
void handler_func(int sig){
	int c = fork();
    if(c > 0){
        return;
    }
    else{
        printf("----- WELCOME TO ECHO SERVER-------");
        int sfd = client_sfd[0];
        for(int i=0;i<9;i++){
            client_sfd[i] = client_sfd[i+1];
            if(client_sfd[i] != 0)
            close(client_sfd[i]);
        }

        char buffer[2048];
        read(sfd, buffer, 2048);
        strcat(buffer, " from echo server");
        write(sfd, buffer, 2048);
    }
}

// Thread function
void* func1(void* args){
    struct thread_args* arg = (struct thread_args*)args;
    int sfd = arg->sfd;
    struct sockaddr_in address = arg->address;
    int addrlen = sizeof(address);
    while(1){
        int nsfd;
        if ((nsfd = accept(sfd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 
        else{
            int i=0;
            while(client_sfd[i] != 0){
                i++;
            }
            client_sfd[i] = nsfd;
        }
    }
}
int main(){
    // pid for signalling from P4
    // ftok to generate unique key
    key_t key1 = ftok("x",65);
  
    // shmget returns an identifier in shmid
    int shmid1 = shmget(key1,1024,0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    char *data1 = (char*) shmat(shmid1,(void*)0,0);
    sprintf(data1,"%d", getpid());


    int p1[2];
    int c = fork();
    if(c == 0){
        close(p1[0]);
        dup2(p1[1], 1);
        close(p1[1]);

        char *args[] = {"./p1", NULL};
        execv(args[0], args);
    }

    mkfifo("green", 0666);
    int p2 = open("green", O_RDONLY);


    //configure to take this info from config file
    
    int sfd;
    int port_num = 6060;
    struct sockaddr_in address;
    
    int opt = 1;
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
    
    signal(SIGUSR1, handler_func);

    // ----------------------------------------- //
    // -------- POLLING -------------//
    
    
    fd_set rfds;
    addrlen = sizeof(address); 
    for(int i=0;i<10;i++){
        client_sfd[i] = 0;
    }

    // create a separate thread to accept clients
    pthread_t thread;
    pthread_create(&thread,NULL,func1,NULL);

printf("hello");
    int poll_sfd[4];
    poll_sfd[0] = p1[0];
    poll_sfd[1] = p2;
    poll_sfd[2] = fileno(stdin);


    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);
        int ma = -1;
        for(int i=0;i<3;i++){
            FD_SET(poll_sfd[i], &rfds);
            if(ma < poll_sfd[i]){
                ma = poll_sfd[i];
            }
        }

		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		//After the select call, rfds set has been modified to contain those sfd that are ready to read
		
        //Check if all A are being used rn
        // cnt suggests can we take another connection? or are all being used rn

        if(count>0){
			int free=0;
			for(int i=0;i<3;i++){	
                // Check is sfd[i] is a part of rfds
                if(FD_ISSET(poll_sfd[i],&rfds)){
                    char buffer[2048];
					read(poll_sfd, buffer, 2048);

                    for(int it=0;it<10;it++){
                        if(client_sfd[i] != 0)
                            write(client_sfd[i], buffer, 2048);
                    }
				}
			}
		}
	}

}