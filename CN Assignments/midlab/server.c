#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include<poll.h>
#include<semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>   
#include<pthread.h>  
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/select.h>
#include<time.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/select.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<errno.h>
#include<netinet/if_ether.h>
#include<net/ethernet.h>
#include<netinet/ether.h>
#include<netinet/udp.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#define __USE_BSD	
#define __FAVOR_BSD	
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include <string.h>
#include <fcntl.h> 
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h> 
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int client_connections[100];
int top = -1;
int start = 0;
int sfd;
struct sockaddr_in address;

void thread_func(void* args){
    
    //int cnt = 0;
    //fd_set rfds;
    
    while(1){
        //int cnt = 0;
		//FD_ZERO(&rfds);
		//FD_SET(sfd,&rfds);
		
		//int ma=-1;
        //if(ma < sfd){
            //ma = sfd;
        //}

		//int count = select(ma+1,&rfds,NULL,NULL,NULL);
		//After the select call, rfds set has been modified to contain those sfd that are ready to read
		int addrlen = sizeof(address);
        //if(count>0){
            //if(FD_ISSET(sfd,&rfds)){	
                int nsfd;
                if ((nsfd = accept(sfd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) { 
                    perror("accept"); 
                    exit(EXIT_FAILURE); 
                } 
                else{
                    printf("Client connection accepted!\n");
                    top++;
                    client_connections[top] = nsfd;
				}	
			//}
		//}
	}
    
}

void P3_process(){
    int c1 = fork();
    if(c1 > 0){
        printf("Echo server created and client sent!\n");
    }
    else{
        for(int i=start+1;i<=top;i++){
            close(client_connections[i]);
        }
        dup2(client_connections[start], 2);
        start++;
        char *args[] = {"./e", NULL};
        execv(args[0], args);
    }
}

int main(){

    // Open 3 pipes to connect with P1 P2 P3

    int pfd1[2], pfd2[2], pfd3[2];
    
    pipe(pfd1);
    pipe(pfd2);
    pipe(pfd3);
    printf("Pipe is created!\n");
    int c1 = fork();

    if(c1 > 0){
        close(pfd1[1]);
        printf("Process P1 created!\n");
    }
    else{
        close(pfd1[0]);

        dup2(pfd1[1], 1);
        close(pfd1[1]);
        char *args[] = {(char *)("./p1"), NULL};
		execv(args[0], args);
    }

    int c2 = fork();
    if(c2 > 0){
        close(pfd2[1]);
        printf("Process P2 created!\n");
    }
    else{
        close(pfd2[0]);

        dup2(pfd2[1], 1);
        //close(pfd2[1]);
        char *args[] = {(char *)("./p2"), NULL};
		execv(args[0], args);
    }

    int c3 = fork();
    if(c3 > 0){
        close(pfd3[1]);
        printf("Process P3 created!\n");
    }
    else{
        close(pfd3[0]);

        dup2(pfd3[1], 1);
        //close(pfd3[1]);
        char *args[] = {(char *)("./p3"), NULL};
        execv(args[0], args);
    }

    // Create sfd for client connections
    int port_num;
    

    port_num = 6060;
    
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
		
    addrlen = sizeof(address);
    pthread_t thread;
    pthread_create(&thread, NULL, thread_func, NULL);

    // Opening rsfd
    int rsfd;
    struct sockaddr_in addr, cliaddr;
    int len = sizeof(cliaddr);
    if ((rsfd = socket(AF_INET, SOCK_RAW, 2)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(rsfd, (const struct sockaddr *)&addr,
         sizeof(addr));
    
    printf("rsfd created\n");


    // Poll on the 3 pipes for input
    int cnt = 0;
    fd_set rfds;
    addrlen = sizeof(address); 
    sleep(10);
    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);
		FD_SET(pfd1[0],&rfds);
		FD_SET(pfd2[0],&rfds);
		FD_SET(pfd3[0],&rfds);

		int ma=-1;
        if(ma < pfd1[0]){
            ma = pfd1[0];
        }

        if(ma < pfd2[0]){
            ma = pfd2[0];
        }

        if(ma < pfd3[0]){
            ma = pfd3[0];
        }

		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		//After the select call, rfds set has been modified to contain those sfd that are ready to read

        if(count>0){
            char buf[2048];
			if(FD_ISSET(pfd1[0], &rfds)){
                printf("Received input on pipe 1\n");
                read(pfd1[0], buf, 2048);
                printf("Buffer is: %s\n", buf);
                char *str = "Hello World" ;
                sendto(rsfd, buf,sizeof(buf), 0, (const struct sockaddr *)&addr, sizeof(addr));
            }
            
            if(FD_ISSET(pfd2[0], &rfds)){
                printf("Received input on pipe 2\n");
                read(pfd2[0], buf, 2048);
                printf("Buffer is: %s\n", buf);
                char str[2048] = "Hello World" ;
                sendto(rsfd, buf,sizeof(buf), 0, (const struct sockaddr *)&addr, sizeof(addr));
            }

            if(FD_ISSET(pfd3[0], &rfds)){
                printf("Received input on pipe 3\n");
                read(pfd3[0], buf, 2048);
                if(start <= top)
                    P3_process();
                else
                    printf("No clients to give!\n");
            }
		}
	}
    /*
    struct pollfd fds[3];

    memset(fds, 0 , sizeof(fds));

    fds[0].fd = pfd1[0];
    fds[1].fd = pfd2[0];
    fds[2].fd = pfd3[0];
    fds[0].events |= POLLIN;
    fds[1].events |= POLLIN;
    fds[2].events |= POLLIN;
    
    int timeout = (3 * 60 * 1000);
    while(1){
        printf("Waiting on input from pipes\n");
        int rc = poll(fds, 3, timeout);
        if(rc < 0){
            perror("poll failed");
            break;
        }
        if (rc == 0){
            printf("  poll() timed out.  End program.\n");
            break;
        }

        for (int i=0;i<3;i++){
            if(fds[i].revents == 0)
                continue;

            if(fds[i].revents & POLLIN){
                if(i == 2){
                    
                }
                else{
                    char buf[2048];
                    if(i == 0){
                        printf("Received input on pipe 1\n");
                        read(pfd1[0], buf, 2048);
                    }
                    else{
                        printf("Received input on pipe 2\n");
                        read(pfd2[0], buf, 2048);
                    }
                    printf("Buffer is: %s", buf);
                    char *str = "Hello World" ;
                    sendto(rsfd, str,sizeof(str), 0, (const struct sockaddr *)&addr, sizeof(addr));
                }
            }
        }
    }
*/
    
}