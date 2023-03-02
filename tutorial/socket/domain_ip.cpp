#include "csapp.h"

/**
 * This code is extracted from Computer Systems: A Programmer's Perspective
 * 
 * Demonstrates the usage of getaddrinfo() and getnameinfo() helper
*/

int main(int argc, char const *argv[])
{
    struct addrinfo *p, *listp, hints; 
    char buf[100];
    int rc, flags;

    if(argc != 2) { // if not one parameter is provided during program call
        fprintf(stderr, "usage: %s <domain name>\n", argv[0]); // then print
        exit(0);
    }

    /* Get a list of addrinfo records */
    memset(&hints, 0, sizeof(struct addrinfo)); // default all values in hints to 0
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) { // if rc is not 0 => not successfull
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc)); // print error msg from getaddrinfo
        exit(1);
    }

    /* Walk the list and display each IP address */
    flags = NI_NUMERICHOST; /* Display address string instead of domain name */
    for(p = listp; p; p = p->ai_next) { // traverse the linkedlist until reaches NULL
        getnameinfo(p->ai_addr, p->ai_addrlen, buf, 100, NULL, 0, flags);
        printf("%s\n", buf); // stored inside buf var
    }

    /* clean up for no memory leak */
    freeaddrinfo(listp);
    exit(0);
}
