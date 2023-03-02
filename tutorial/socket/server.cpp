#include "csapp.h"

int open_listenfd(const char *port) { 
    /**
     * The socket for server goes through 3 steps to listen for connections:
     * 1. Create a socket (this is default as socket for client [active socket])
     * 2. Binds the socket to socket descriptor (socketfd)
     * 3. Listens to connection by passing in socketfd to listen() function
    */

    int listenfd, optval = 1;
    struct addrinfo hints, *listp, *p;
    /* Get a list of potential server addresses for a stream socket */
    memset(&hints, 0, sizeof(struct addrinfo));
    /* types of socket: https://www.ibm.com/docs/en/zos/2.1.0?topic=concepts-introducing-tcpip-selecting-sockets */
    hints.ai_socktype = SOCK_STREAM; /* accept stream socket */
    /* AI_PASSIVE is set so it returns address for sockets for server */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* on any IP address */
    hints.ai_flags |= AI_NUMERICSERV; /* using port numbers */
    
    /**
    * What does getaddrinfo do? => https://jameshfisher.com/2018/02/03/what-does-getaddrinfo-do/
    * getaddrinfo doc => https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
    * The getaddrinfo() function translates the name of a service location (for example, a host name) and/or service name 
    * and returns a set of socket addresses and associated information to be used in creating a socket with which to address the specified service.
    */
    getaddrinfo(NULL, port, &hints, &listp);
    /**
     * why does getaddrinfo() return a linkedlist instead of a single address? (TCP/IP Sockets in C by Michael J. Donahoo)
     * 1.  First reason is that for each combination of host and service, there might be several 
     *     different combinations of address family (v4 or v6) and socket type/protocol (stream/TCP or datagram/UDP) 
     *     that represent possible endpoints.  
     * 2.  The second reason is that a hostname can map to multiple IP addresses.
     * */ 

    /* Walk the list for one that we can bind to */
    printf("creating a listening socket...\n");
    for(p = listp; p; p = p->ai_next) {
        // ai_protocol = 6 (TCP), sockettype = 1 (STREAM socket), family = 30 (IPv6)
        printf("protocol: %d, socket-type: %d, address-family: %d \n", listp->ai_protocol, listp->ai_socktype, listp->ai_family);
        /* Create a socket descriptor */
        if((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            printf("failed, trying next socket...\n");
            continue;
        }

        /* The setsockopt() set the options for the specified protocol level */
        // In this case, the option that is being changed is at the socket level (SOL_SOCKET) 
        // and it is set to allow for local address to be reusable
        printf("set socket options...\n");
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

        /* bind the descriptor to the address */
        printf("binding the address to the socket...\n");
        if(bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;

        printf("failed to bind, going to next socket...\n");
        close(listenfd); /* if bind failed, closes the descriptor */
    }
    /* clean up */
    freeaddrinfo(listp);
    if(!p) return -1; /* No address */

    /* make the socket a listening socket, ready to accept connection request */
    if(listen(listenfd, 1024) < 0) {
        close(listenfd); /* if returns < 0, then it fails so closes descriptor*/
        return -1;
    }
    printf("returning listener descriptor...\n");
    return listenfd;
};

void echo(int connfd) {
    size_t n;
    char buf[MAXLINE];
    rio_t rio(connfd);

    /* escape rio_readlineb after client terminates connection */
    while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", (int) n);
        rio_writen(connfd, buf, n);
    }
}
