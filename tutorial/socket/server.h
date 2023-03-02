#ifndef TUTORIAL_SOCKET_SERVER_H
#define TUTORIAL_SOCKET_SERVER_H

#include "csapp.h"

/// @brief open socket descriptor for listening on localhost
/// @param port port number
/// @return descriptor key
int open_listenfd(const char *port);

void echo(int connfd);

#endif