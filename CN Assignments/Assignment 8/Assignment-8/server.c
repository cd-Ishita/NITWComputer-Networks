#include <stdio.h>get
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

#define PORT "3590"  // the port users will be connecting to
#define MAX_SIZE 1024
#define BACKLOG 10   // how many pending connections queue will hold

int open_socket(){
    struct addrinfo hints;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    int rv, sfd;
    struct addrinfo *servinfo;
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    struct addrinfo *p;
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        int yes = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sfd);
            perror("server: bind");
            continue;
        }

        break;
    }
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    return sfd;
}
int main(void){
    
    int sfd = open_socket();
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int x = getsockname(sfd, &address, &address_len);

    printf("Local IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("Local port is: %d\n", (int) ntohs(address.sin_port));

    struct sockaddr_storage client_address; // connector's address information
    socklen_t sin_size;
    int new_fd; //new connection
    char s[INET6_ADDRSTRLEN];

    sin_size = sizeof client_address;
    new_fd = accept(sfd, (struct sockaddr *)&client_address, &sin_size);
    if (new_fd == -1) {
        perror("accept");
        //continue;
        return 1;
    }
    
    
    /*
    struct sockaddr_in my_addr;
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd == -1){
        printf("socket connect error");
        return 1;
    }

    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(3590);

    int bstats = bind(sfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    if(bstats == -1){
        printf("bind error");
        return 1;
    }

    int list_stat = listen(sfd, 10);
    if(list_stat == -1){
        printf("listen error");
        return 1;
    }

    int len = sizeof(my_addr);
    int new_fd = accept(sfd, (struct sockaddr *)&my_addr, (socklen_t*)&len);

    if(new_fd == -1){
        printf('accept error');
        return 1;
    }

*/
    int n;
    FILE *fp;
    char *filename = "P1.cpp";
    char buffer[MAX_SIZE];

    fp = fopen(filename, "w");
    while (1) {
        n = recv(new_fd, buffer, MAX_SIZE, 0);
        if (n <= 0){
            break;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, MAX_SIZE);
    }
    printf("File is written\n");
    close(sfd);
    /*
    char* command;
    strcpy(command, "g++ -o P1 P1.cpp");
    system(command);
    */
    
    printf("Compiling the code\n");
    int c1 = fork();
    if(c1 > 0){
        wait(NULL);
    }
    else{
        char *args[] = {"cc", "-o", "./P1", "P1.cpp", NULL};
        execv("/usr/bin/cc", args);
    }

    printf("Fork part\n");
    
    
    //Forks
    int c = fork();
    if(c>0){
        wait(NULL);

        FILE* pout_fd;
        FILE* ot_fd;
        pout_fd = fopen("Pout.txt", "r");
        ot_fd = fopen("OT.txt", "r");

        char ch1 = getc(pout_fd);
        char ch2 = getc(ot_fd);

        int error = 0;

        while (ch1 != EOF && ch2 != EOF){
            if (ch1 != ch2){
                error++;
                break;
            }

            ch1 = getc(pout_fd);
            ch2 = getc(ot_fd);
        }

        printf("Enter char: ");
        int d;
        scanf("%d", d);
        int sfd1 = open_socket();
        struct sockaddr_storage client_address; // connector's address information
        socklen_t sin_size;
        char s[INET6_ADDRSTRLEN];

        sin_size = sizeof client_address;
        int nfd = accept(sfd1, (struct sockaddr *)&client_address, &sin_size);
        if (nfd == -1) {
            printf("accept");
            //continue;
            return 1;
        }

        if(error >0 ){
            char buf[2048];
            strcpy(buf, "Test Case Failed!\n");
            send(nfd, buf, 2048, 0);
            printf("Server sent: '%s'\n",buf);
        }
        else{
            char buf[2048];
            strcpy(buf, "Test Case Passed!\n");
            send(nfd, buf, 2048, 0);
            printf("Server sent: '%s'\n",buf);
        }
    }
    else{
        FILE *itfd;
        FILE *poutfd;

        itfd = fopen("IT.txt","r");
        poutfd = fopen("Pout.txt", "w");

        dup2(itfd, 0);
        dup2(poutfd, 1);

        close(itfd);
        close(poutfd);
        char *args[] = {"./P1", NULL};
		execv(args[0], args);
    }

    //freeaddrinfo(servinfo); // all done with this structure

    return 0;
    
}