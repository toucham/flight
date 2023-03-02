#include "rio.h"

// Unbuffered I/O functions
ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
    size_t nleft = n; /* counter for # of bytes to read */
    ssize_t nread; /* current bytes read */
    char *bufp = (char *) usrbuf; /* to store the bytes being read */

    while(nleft > 0) {
        /**
         * read() returns # of bytes read if OK, 0 on EOF, -1 on error 
         * but it doesn't try to reach EOF and stops after reading the designated chunk size
        */
        if((nread = read(fd, bufp, nleft)) < 0) { // if error
            /* errno is a preprocessor macro for error indication => https://en.cppreference.com/w/cpp/error/errno */
            if(errno == EINTR) nread = 0; // if interrupted by sig handler => reread from beginning
            else return -1; // errno set by read()
        } else if(nread == 0) { // if reaches EOF
            break;
        }
        nleft -= nread; // decrease the # of bytes left to be read
        bufp += nread; // moves the pointer to the next empty address
    }

    return (n - nleft); // return >= 0
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
    size_t nleft = n; /* counter for # of bytes to write */
    ssize_t nwritten; /* # of bytes left to write */
    char *bufp = (char *) usrbuf; /* to get the bytes to write */

    while(nleft > 0) {
        if((nwritten = write(fd, bufp, nleft)) <= 0) {
            if(errno == EINTR) nwritten = 0;
            else return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }

    return n;
}

// Buffered I/O functions
ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) // application buffer ver. of read()
{
    int cnt;
    /* if buffer is empty, read from descriptor */
    while(rp->rio_cnt <= 0) {
        /* read data of size rio_buf from rio_fd */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf)); // read() returns the # of bytes read
        if(rp->rio_cnt < 0) { /* encounter error */
            if(errno != EINTR) return -1; /* not interrupted by sig handler */
        } else if(rp->rio_cnt == 0) {
            return 0; /* finished reading everything */
        } else {
            rp->rio_bufptr = rp->rio_buf; /* reset buffer ptr*/
        }
    }

    /* copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
    cnt = n > rp->rio_cnt ? rp->rio_cnt : n;
    memcpy(usrbuf, rp->rio_bufptr, cnt); // copy the values of [cnt] bytes from src to dest (rio_buf -> usrbuf)
    rp->rio_bufptr = rp->rio_bufptr + cnt; // moves pointer to the rest of the unread bytes
    rp->rio_cnt -= cnt;
    return cnt; // returns the # of bytes read
}

/* read line by line until reaches EOF */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
    int n, rc;
    char c, *bufp = (char *) usrbuf;

    for(n = 1; n < maxlen; n++) {
        if((rc = rio_read(rp, &c, 1)) == 1) { /* read each byte*/
            *bufp++ = c;
            if(c == '\n') { /* checks for newline char */
                n++;
                break; /* stops reading after found newline char */
            }
        } else if(rc == 0) {
            if(n == 1) {
                return 0; /* EOF, no data read */
            } else {
                break; /* EOF, some data was read */
            }
        } else {
            return -1; /* Error */
        }
    }
    *bufp = 0;
    return n-1;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *) usrbuf;

    while(nleft > 0) {
        if((nread = rio_read(rp, bufp, nleft)) < 0) {
            return -1; /* error */
        } else if(nread == 0) {
            break;
        }
        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft);
}