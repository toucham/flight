#include "csapp.h"

/// @brief return the file type of a file
/// @param filename name of the file to check
/// @param filetype store the file type
void get_filetype(char* filename, char* filetype) {
    if(strstr(filename, ".html")) {
        strcpy(filetype, "text/html");
    } else if(strstr(filename, ".gif")) {
        strcpy(filetype, "image/gif");
    } else if(strstr(filename, ".png")) {
        strcpy(filetype, "image/png");
    } else if(strstr(filename, ".jpg")) {
        strcpy(filetype, "image/jpeg");
    } else {
        strcpy(filetype, "text/plain");
    }
}

/// @brief only print the request headers
/// @param rp read buffer
void read_requesthdrs(rio_t* rp) {
    char buf[MAXLINE];

    rio_readlineb(rp, buf, MAXLINE);
    printf(YEL "$ ----- read_requesthdrs() -----\n" RESET);
    while(strcmp(buf, "\r\n")) { // the headers are ignored
        rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    printf(YEL "$ ------ read_requesthdrs ------\n" RESET);
}

/// @brief forking a child process and executes the CGI program
/// @param fd socket desciptor
/// @param filename CGI program name
/// @param cgiargs arguments for CGI program
void serve_dynamic(int fd, char* filename, char* cgiargs) {
    char buf[MAXLINE], *emptylist[] = {NULL};

    /* returns first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));

    if(fork() == 0) { /* child */
        /* real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(fd, STDOUT_FILENO);
        extern char** environ;
        execve(filename, emptylist, environ); /* run cgi program */
    }

    wait(NULL);
}

/// @brief serve static files
/// @param fd socket descriptor
/// @param filename file name
/// @param size file size
void serve_static(int fd, char* filename, int size) {
    int srcfd; /* descriptor to the opened file */
    void *srcp; /* address to the map */
    char filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n", size);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: %s\r\n\r\n", filetype);
    rio_writen(fd, buf, strlen(buf));


    /* send response body to client */
    srcfd = open(filename, O_RDONLY, 0);
    /* creates a new mapping in the virtual address space of the calling process */
    srcp = mmap(0, size, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd); /* closes the file descriptor */
    rio_writen(fd, srcp, size); /* writes the entire file to the socket */
    munmap(srcp, size); /* deletes the mapping with addr from [srcp] */
}

/// @brief parse uri for HTTP GET request
/// @param uri
/// @param filename store file name that is called
/// @param cgiargs store args for CGI program if dynamic content
/// @return returns 1 if static or 0 if dynamic
int parse_uri(char* uri, char* filename, char* cgiargs) {
    printf(YEL "$ ----- parse_uri() -----\n" RESET);
    char *ptr;
    if(!strstr(uri, "cgi-bin")) { /* static content */
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if(uri[strlen(uri)-1] == '/') {
            strcat(filename, "index.html");
        }
        printf("static filename: %s\n", filename);
        printf(YEL "$ ------ parse_uri ------\n" RESET);
        return 1;
    } else { /* dynamic content */
        ptr = index(uri, '?');
        if(ptr) {
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        } else {
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        printf("dynamic filename: %s\n", filename);
        printf(YEL "$ ------ parse_uri ------\n" RESET);
        return 0;
    }
}


/// @brief sends HTTP response with static content of error message
/// @param fd socket descriptor
/// @param cause cause of error
/// @param errnum HTTP error code
/// @param shortmsg short error message
/// @param longmsg long error message
void clienterror(int fd, char* cause, const char* errnum, const char* shortmsg, const char* longmsg) {
    char buf[MAXLINE], body[MAXBUF];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    rio_writen(fd, buf, strlen(buf));
}

/// @brief parse HTTP request
/// @param fd socket descriptor
void doit(int fd) {
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];

    /* read request line and headers */
    rio_t rio(fd); /* read buffer */
    rio_readlineb(&rio, buf, MAXLINE);
    printf("%s\n", rio.rio_buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not implemented", "Tiny does not implement this method");
        return;
    }
    read_requesthdrs(&rio);

    /* parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs); /* check for type of the request: dynamic or static */
    if(stat(filename, &sbuf) < 0) {
        clienterror(fd, filename, "404", "Not found", "Tiny couldn't find this file");
        return;
    }

    if(is_static) {
        if(!S_ISREG(sbuf.st_mode) || !(S_IRUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size);
    } else {
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }

}

int main(int argc, char const *argv[])
{
    printf(GRN "$ Starting server...\n" RESET);
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    sockaddr_storage clientaddr;

    /* check command-line arguments */
    if(argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = open_listenfd(argv[1]);
    printf(GRN "$ Listening on port: %s\n" RESET, argv[1]);
    
    while(1) {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (sockaddr *) &clientaddr, &clientlen);
        getnameinfo((sockaddr *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        printf(GRN "$ Accepted connection from (%s, %s)\n" RESET, hostname, port);
        printf(YEL "$ ----- doit() -----\n" RESET);
        doit(connfd); /* for handling HTTP transaction */
        printf(YEL "$ ------ doit ------\n" RESET);
        close(connfd);
    }

    return 0;
}