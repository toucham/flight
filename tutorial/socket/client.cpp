#include "csapp.h"

/* Instantiating an active TCP/IP socket to connect to server */
int open_clientfd(const char *hostname, const char *port) {
    int clientfd;
    struct addrinfo hints, *listp, *p;

    /* Get a list of potential server addresses */
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM; /* Open a connection */
    hints.ai_flags = AI_NUMERICHOST; /* using a numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG; /* Returns IPv4 or IPv6 according to the host */
    /* getaddrinfo() sets listp as a linkedlist of sockaddr */
    getaddrinfo(hostname, port, &hints, &listp);

    for(p = listp; p; p = p->ai_next) {
        /* Create a socket descriptor */
        printf("protocol: %d, socket-type: %d, address-family: %d \n", listp->ai_protocol, listp->ai_socktype, listp->ai_family);
        if((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) { // protocol is dependent on the socket
            printf("failed \n");        
            continue; // if failed, then try next socket
        }
        printf("connecting using the client descriptor...\n");
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) break; // break if success

        printf("failed to connect, trying next address...\n");
        close(clientfd); // closes the descriptor
    }

    /* clean up */
    freeaddrinfo(listp);
    if(!p) {
        printf("failed to connect!\n");
        return -1;
    } else {
        printf("connect successfully!\n");
        return clientfd;
    }
}

