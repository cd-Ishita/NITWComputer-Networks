#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

int main(int argc, char *argv[]){
    
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *servinfo;
    int rv;
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    struct addrinfo *p;
    int sfd;
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sfd);
            
            continue;
            }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    /* char s[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s); */

    freeaddrinfo(servinfo); // all done with this structure
    
    struct sockaddr_in address;
    int address_len = sizeof(address);
    int x = getsockname(sfd, &address, &address_len);

    printf("Local IP address is: %s\n", inet_ntoa(address.sin_addr));
    printf("Local port is: %d\n", (int) ntohs(address.sin_port));

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

    char buf[2048];
    printf("Client 2 wants input: \n");
    scanf("%s", buf);
    send(sfd, buf, 2048, 0);
    printf("client2: sent '%s'\n",buf);

    close(sfd);

    return 0;
}