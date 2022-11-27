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

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10   // how many pending connections queue will hold

void sigchld_handler(int s){
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
/*
void *get_in_addr(struct sockaddr *sa){
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
*/
int main(void){
    
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

    struct sockaddr_in address;
    int address_len = sizeof(address);
    int x = getsockname(sfd, &address, &address_len);

    printf("Local IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("Local port is: %d\n", (int) ntohs(address.sin_port));


    //Now accept

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    /*
    struct sigaction sa;
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
*/
    printf("Server: waiting for connections...\n");

    struct sockaddr_storage client_address; // connector's address information
    socklen_t sin_size;
    int new_fd; //new connection
    char s[INET6_ADDRSTRLEN];

    while(1) {  // main accept() loop
        sin_size = sizeof client_address;
        new_fd = accept(sfd, (struct sockaddr *)&client_address, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        /*
        inet_ntop(client_address.ss_family,
            get_in_addr((struct sockaddr *)&client_address),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        */
        //CHECK IF WE NEED THIS

        socklen_t len;
        struct sockaddr_storage addr;
        char ipstr[INET6_ADDRSTRLEN];
        int port;

        len = sizeof addr;
        getpeername(sfd, (struct sockaddr*)&addr, &len);

        // deal with both IPv4 and IPv6:
        if (addr.ss_family == AF_INET) {
            struct sockaddr_in *sfd = (struct sockaddr_in *)&addr;
            port = ntohs(sfd->sin_port);
            inet_ntop(AF_INET, &sfd->sin_addr, ipstr, sizeof ipstr);
        } else { // AF_INET6
            struct sockaddr_in6 *sfd = (struct sockaddr_in6 *)&addr;
            port = ntohs(sfd->sin6_port);
            inet_ntop(AF_INET6, &sfd->sin6_addr, ipstr, sizeof ipstr);
        }

        printf("Peer IP address: %s\n", ipstr);
        printf("Peer port      : %d\n", port);

        char message_recv[2048];
        int bytes_recv = recv(new_fd, message_recv, 2047, 0);
        message_recv[bytes_recv] = '\0';
        printf("Message received is: %s\n", message_recv);

        if (!fork()) { // this is the child process
            close(sfd); // child doesn't need the listener
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    freeaddrinfo(servinfo); // all done with this structure

    return 0;
}