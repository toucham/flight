<h1> Simple HTTP Server Framework with C++ </h1>

This is a step-by-step process of my implementations of a simple HTTP web server.

<h2>Why?</h2>
This repository serves as a learning experience for me to practice programming in C++. Having only worked with interpreted high-level programming language like javascript/typescript and python, I want to try programming in low-level languages to learn and review as much as possible about computer systems, while also refining my foundations as a programmer. Furthermore, this will also serves as my first programming blog post.


<h2>Table of Contents</h2>

- [C++ Web Server Implementations](#c-web-server-implementations)
  - [Open TCP socket for connection](#open-tcp-socket-for-connection)
    - [Socket interface](#socket-interface)
    - [IP Address](#ip-address)
  - [Parsing HTTP/1.1 \& HTTP/2.0 requests](#parsing-http11--http20-requests)
  - [Routings](#routings)
  - [Serve static content](#serve-static-content)



# C++ Web Server Implementations
Now that we have a basic understanding of computer networking, we can try implementing a HTTP web server with C++.
## Open TCP socket for connection 
HTTP/1.1 and HTTP/2 are all application-layer protocol that is implemented under TCP/IP. In order to establish a connection between a web server and client, both entities have to open a TCP socket in order to send HTTP requests and receive HTTP responses.

<!-- Each computer, containing a set of sockets/hosts, is assigned with an unsigned 32-bit integer or IPv4 address, that is used by the Internet Protocol (IP) in the network-layer to identify the host and deliver the datagram to the correct host. -->

### Socket interface
The socket interface/API is a set of functions that is used with the Unix I/O functions to build network application. It is implemented on most modern systems like Window and Macintosh.
### IP Address
The IP address is stored inside a structure, instead of being a scalar type, because of the early implementations of the socket interface that is too late to be changed due to enormous installed base applications.
```cpp
/* IP address structure */
struct in_addr {
  uint32_t s_addr; // Address in network byte order (big-endian)
}
```


## Parsing HTTP/1.1 & HTTP/2.0 requests

## Routings

## Serve static content
