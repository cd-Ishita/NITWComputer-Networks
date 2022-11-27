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
#include<sys/select.h>

void S1(char buffer[2048], int nsfd, int type){
    printf("Service 1 in use\n");
    int i=0;
    while(buffer[i] != '\0'){
        if((int)buffer[i] >= 97 && (int)buffer[i] <= 122)
        buffer[i] = (char)((int)buffer[i] - 32);
        i++;
    }

    if(type == 0){
        // tcp connection
        write(nsfd, buffer, 2048);
    }
    else{
        //udp connection
        struct sockaddr_in cli_addr;
        int cli_len = sizeof(cli_addr);
        sendto(nsfd, buffer, 2048, 0, (struct sockaddr*)&cli_addr, cli_len);
    }
}

void S2(char buffer[2048], int nsfd, int type){
    printf("Service 2 in use\n");
    int i=0;
    while(buffer[i] != '\0'){
        if((int)buffer[i] >= 65 && (int)buffer[i] <= 90)
        buffer[i] = (char)((int)buffer[i] + 32);
        i++;
    }

    if(type == 0){
        // tcp connection
        write(nsfd, buffer, 2048);
    }
    else{
        //udp connection
        struct sockaddr_in cli_addr;
        int cli_len = sizeof(cli_addr);
        sendto(nsfd, buffer, 2048, 0, (struct sockaddr*)&cli_addr, cli_len);
    }
}

void S3(char buffer[2048]){
    printf("Service 3 in use\n");
    printf("Data from Client: %s", buffer);
}


int main(){
    // Number of sockets
    int num = 5;
    int sfd[num];
    int port_num[num];
    struct sockaddr_in address[3];

    //configure to take this info from config file
    port_num[0] = 6060;
    port_num[1] = 7070;
    port_num[2] = 8080;
    port_num[3] = 9090;
    port_num[4] = 10010;

    for(int i=0;i<3;i++){
        int opt = 1;
        int addrlen = sizeof(address[i]);
        
        // Creating socket file descriptor
        if ((sfd[i] = socket(AF_INET, SOCK_STREAM, 0)) == 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        
        // Forcefully attaching socket to the port 8080
        if (setsockopt(sfd[i], SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address[i].sin_family = AF_INET;
        address[i].sin_addr.s_addr = INADDR_ANY;
        address[i].sin_port = htons(port_num[i]);
        
        // Forcefully attaching socket to the port 8080
        if (bind(sfd[i], (struct sockaddr *)&address[i], sizeof(address[i]))<0){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(sfd[i], 5) < 0){
            perror("listen");
            exit(EXIT_FAILURE);
        }
        printf("sfd%d is listening for connections\n", i+1);
    }
    // -------------------- udp sockets ----------------------
    struct sockaddr_in servaddr[2], cliaddr[2];
    
    for(int i=0;i<2;i++){
        // Creating socket file descriptor
        if ( (sfd[3+i] = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }
        
        memset(&servaddr[i], 0, sizeof(servaddr[i]));
        memset(&cliaddr[i], 0, sizeof(cliaddr[i]));
        
        // Filling server information
        servaddr[i].sin_family    = AF_INET; // IPv4
        servaddr[i].sin_addr.s_addr = INADDR_ANY;
        servaddr[i].sin_port = htons(port_num[3+i]);
        
        // Bind the socket with the server address
        if (bind(sfd[3+i], (const struct sockaddr *)&servaddr[i], sizeof(servaddr[i])) < 0 ){
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        printf("sfd%d is ready for connections\n", 4+i);
    }

    // -------------------- select ----------------

    
    fd_set rfds;

    while(1){
        int cnt = 0;
		FD_ZERO(&rfds);
		FD_SET(sfd[0],&rfds);
		FD_SET(sfd[1],&rfds);
		FD_SET(sfd[2],&rfds);
        FD_SET(sfd[3], &rfds);
        FD_SET(sfd[4], &rfds);

		int ma=-1;
		for(int i=0;i<4;i++){
			if(ma<sfd[i])
	    		ma=sfd[i];
	    }
		int count = select(ma+1,&rfds,NULL,NULL,NULL);
		//After the select call, rfds set has been modified to contain those sfd that are ready to read
		
        //Check if all A are being used rn
        // cnt suggests can we take another connection? or are all being used rn

        if(count>0){
			int free=0;
			for(int i=0;i<num;i++){
				
                if(FD_ISSET(sfd[i],&rfds)){	
                    if(i<3){
                        int nsfd;
                        int addrlen = sizeof(address); 
                        if ((nsfd = accept(sfd[i], (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) { 
                            perror("accept"); 
                            exit(EXIT_FAILURE); 
                        } 
                        else{
                            printf("Client connected!\n");
                            char buffer[2048], buffer1[2048];
                            read(nsfd, buffer, 2048);
                            read(nsfd, buffer1, 2048);
                            if(buffer[1] == '1'){
                                S1(buffer1, nsfd, 0);
                            }
                            else if(buffer[1] == '2'){
                                S2(buffer1, nsfd, 0);
                            }
                            else{
                                S3(buffer1);
                            }
                        }
                    }
                    else{

                        printf("Client connected!\n");
                        char buffer[2048], buffer1[2048];
                        struct sockaddr_in cliaddr1;
                        int len = sizeof(cliaddr1);
                        recvfrom(sfd[i], (char *)buffer, 2048, MSG_WAITALL, ( struct sockaddr *) &cliaddr1,&len);
                        recvfrom(sfd[i], (char *)buffer1, 2048, MSG_WAITALL, ( struct sockaddr *) &cliaddr1,&len);
                        
                        if(buffer[1] == '1'){
                            S1(buffer1, sfd[i], 1);
                        }
                        else if(buffer[1] == '2'){
                            S2(buffer1, sfd[i], 1);
                        }
                        else{
                            S3(buffer1);
                        }
                    }
                    
                }
                else{
                    free++;
                }	
			}
			if(free == num){
                // It comes to this if none of the sockets received a connection
			    printf("Timeout reached\n");
            }
		}
	}
}