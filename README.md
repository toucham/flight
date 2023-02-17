<style>
.big-img {
  width: 50%;
  min-width: 360px;
  max-width: 550px;
}
</style>
<h1> Simple HTTP Server Framework with C++ </h1>

This is a step-by-step process of my implementations on a simple HTTP web server based on my readings of a computer system textbook that was recommended during my computer science course in university. 

<h2>Why?</h2>
This repository serves as a learning experience for me to practice programming in C++. Having only worked web full-stack development and programming in interpreted high-level programming language like javascript/typescript and python, I want to try low-level languages with no garbage collector as I want to learn and review as much as possible about computer systems, while also refining my foundations as a programmer.

<br/>

Furthermore, this will also serves as my first programming blog post as I always want to try do more of these posts in the future.

<br/>

<h2>Table of Contents</h2>

- [Background](#background)
  - [Web server](#web-server)
  - [Networks](#networks)
    - [Five Layers Model](#five-layers-model)
    - [Encapsulation](#encapsulation)
    - [Application Layer - HTTP](#application-layer---http)
- [C++ Web Server Implementations](#c-web-server-implementations)
  - [Open TCP socket for connection](#open-tcp-socket-for-connection)
    - [TCP Socket class](#tcp-socket-class)
  - [Parsing HTTP/1.1 \& HTTP/2.0 requests](#parsing-http11--http20-requests)
  - [Routings (RESTful API)](#routings-restful-api)
  - [Serve static content](#serve-static-content)
- [Implement other features](#implement-other-features)
  - [Multithreading server](#multithreading-server)

# Background
The information presented below on computer system and computer network is based on my readings of "Computer Systems: A Programmer's Perspective 3rd Edition" by Randal E. Bryant & David R. O'Hallaron. and "Computer Networking: A Top-Down Approach 8th Edition" by Jim Kurose and Keith Ross.

If any of the information presented below is incorrect, please let me know through PR of changes in the README.md and I will merge it. Thank you.

## Web server
A web server follows the client-server model (Figure 1), where there can be one server process (not necessarily one) and one or more client processes. The server process manages the some kind of resources (database or service) that is used to process the requests made by its clients. The fundamental operation made in this model is <i>transaction</i> as shown in Figure 1. This operation consists of four steps:

1. Client initiates a transaction by sending a request to the server (e.g. HTTP Requests).
2. Server receives the request and process it as necessary. (e.g. Client might request their user information, that is stored in the database that is accessible by the server).
3. Server replies back to the client with a response (e.g. HTTP Response).
4. The response could be in some kind of format that needs the client to process in order to use or understand it (e.g. HTTP response body could contain JSON with specific type or it could be a encoded string that needs decoding).

<center>
   <img src="./assets/client-server-model.svg" />
</center>

<b>FYI:</b> This is the most basic and simple model of web server. In reality, a web server for just only one website can have many server processes, either of same or different service due to the nature of the language it is coded in (this is called a <i>host</i>). For example, NodeJs servers is a single-thread language and therefore, if required to scale, it needs to be done by deploying several processes instead of using threads to process multiple incoming requests. Furthermore, there are many other models that are extended from this model like microservice architecture or BFF models that are used for scaling web servers.

<br />

## Networks
The communication mode between the client and server is through computer network. Essentially, computer network is an interconnected systems between computers and devices, exchanging data and resource. For computers, it connects to the network through network adaptors, which in computer system's perspective, the network adapter is just another I/O device that serves as a data source and sink.

Computer network is a deep and extensive topic that is divided into many abstract levels; therefore, for this project, we will only understand the minimal knowledge required to create a web server. However, if anyone wants to delve deeper in this topic, I really recommend reading ["Computer Networking: A Top-Down Approach 8th Edition" by Jim Kurose and Keith Ross](https://gaia.cs.umass.edu/kurose_ross/index.php).

### Five Layers Model
In computer networking, the format of the messages exchanged between hosts/devices is determined by a set of standardized protocols. For this project, the protocols will be based of the <b>five-layer protocols</b> mentioned in Jim Kurose and Keith Ross textbook (Figure 2). There is also the OSI model that splits the protocols in 7 layers; however, this project will be based of Kurose and Rose model. The five-layers model splits and abstracts different aspects of networking into five layers, which the top four layers have each different protocols on how the messages are formatted. Each layer is used for different purpose in networking.

<center>
   <img src="./assets/five-layers-protocol.svg" />
</center>

### Encapsulation
Hosts send messages to another by encapsulating their messages with each layer's protocol. Each protocol is used to maintain different aspects of computer networking. For example, the frame from link-layer or datagram from network-layer is used for routing the message to its destination, deciding how and where the datagrams/frames are routed.

The transport layer protocol determines how segments (encapsulated application-layer message) are transported between two endpoints (how two sockets communicate with each other).

<center>
   <img src="./assets/encapsulations-network.svg" class="big-img" />
</center>

As shown in the figure above, the host (computer) sends a message by encapsulating it with transport-layer, network-layer, and link-layer protocols. When arrived, the destination host decapsulate the frame to get the message.

<br/>

### Application Layer - HTTP
The layer that will be implemented and focused on is the application layer protocol. It is the layer that controls the communication between two processes. In this case, it is the Hypertext Transfer Protocol (HTTP), an application layer protocol that is commonly used for transferring resources and data between web browser and server. We'll be implementing [HTTP/2](https://www.rfc-editor.org/rfc/rfc9113) and [HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9110) that use the TCP/IP protocols (TCP for transport layer & IP for network layer).

<center>
   <img src="./assets/socket-app-layer.svg" />
</center>

As shown in figure 4, a socket is used to communicate between two processes. It is the interface between the application layer and the transport layer within a host. Many languages, including C++, provide API to access these socket interfaces, which is why we only have to program the application layer.

<b>FYI:</b> Different operating systems can use different socket API; therefore, it is possible that the web server is not runnable on your computer. For this project, we are developing it on MacOS, so we'll be using the POSIX socket from `sys/socket.h` file. To read abit more on different socket libaries, you can read it [here](https://stackoverflow.com/questions/28027937/cross-platform-sockets).

<br/>

# C++ Web Server Implementations

## Open TCP socket for connection 
HTTP/1.1 and HTTP/2 are all application-layer protocol that is implemented under TCP/IP In order to establish connection between a web server and client, both entities have to estalibish a connection through TCP socket in order to send HTTP requests and receive HTTP responses.

### TCP Socket class 


## Parsing HTTP/1.1 & HTTP/2.0 requests

## Routings (RESTful API)

## Serve static content

# Implement other features
To be continued...

## Multithreading server
