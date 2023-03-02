#ifndef RIO
#define RIO

#define RIO_BUFSIZE 8192
#include <stdio.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/// @brief unbuffered input function => read bytes until reaches EOF
/// @param fd file descriptor
/// @param usrbuf buffer to store bytes
/// @param n size of files to read (unsigned long)
/// @return number of bytes if OK, 0 if reaches EOF, -1 if error (signed long)
ssize_t rio_readn(int fd, void *usrbuf, size_t n);

/// @brief unbuffered output function => write bytes until see EOF
/// @param fd file descriptor
/// @param usrbuf buffer to store bytes 
/// @param n size of files (unsigned long)
/// @return number of bytes if OK, -1 if error (signed long)
ssize_t rio_writen(int fd, void *usrbuf, size_t n);

class rio_t {
public:
    int rio_fd; /* descriptor for the internal buffer */
    int rio_cnt; /* # of bytes in the buffer */
    char *rio_bufptr; /* next unread byte in internal buffer */
    char rio_buf[RIO_BUFSIZE]; /* internal buffer */

    rio_t(int fd) {
        this->rio_fd = fd;
        this->rio_cnt = 0;
        this->rio_bufptr = this->rio_buf;
    }
};

/// @brief app buffered input
/// @param rp read buffer
/// @param usrbuf user buffer
/// @param n at most size to read
/// @return returns a short count if the # of requested bytes > # of unread bytes in the read buffer, returns -1 on error, returns 0 on EOF
ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n);

/// @brief app buffered input per line
/// @param rp read buffer
/// @param usrbuf user buffer
/// @param maxlen size of buffer
/// @return returns a short count if the # of requested bytes > # of unread bytes in the read buffer, returns -1 on error
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

#endif