#include "server.h"

int main(int argc, char const *argv[])
{   
    int listenfd, connfd;
    socklen_t clientlen;
    sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if(argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = open_listenfd(argv[1]);
    printf("retrieved listenfd!\n");
    while(true) {
        clientlen = sizeof(sockaddr_storage);
        /* accept() returns the socket address and length of the address */
        connfd = accept(listenfd, (sockaddr *)&clientaddr, &clientlen);
        /* fill in the client_hostname & client_port from*/
        getnameinfo((sockaddr *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        close(connfd);
    }

    exit(0);
    return 0;
}