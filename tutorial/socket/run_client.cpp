#include "client.h"

int main(int argc, char const *argv[])
{
    const char *host, *port;
    char buf[MAXLINE];
    int clientfd;

    if(argc != 3) { // requires host and port arguments when running the program
        fprintf(stderr, "usage %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = argv[2];

    clientfd = open_clientfd(host, port); // retrieve socket descriptor
    if(clientfd == -1) {
        exit(1);
    }
    rio_t rio(clientfd);
    while(fgets(buf, MAXLINE, stdin)) {
        rio_writen(clientfd, buf, strlen(buf)); // sends message to the server
        rio_readlineb(&rio, buf, MAXLINE); // receives message from the server
        fputs(buf, stdout); // prints the message to the standard output (terminal)
    }
    close(clientfd);
    exit(0);
}
