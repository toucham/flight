#include <sys/socket.h>
#define PORT 8080

// socket for accepting TCP/IP segment
class TCPSocket {
    int server;
public:
    TCPSocket() {
        // AF_INET = indicate that we're using 32-bit IP addresses
        // SOCK_STREAM = indicates that the socket will be an endpoint for a connection
        server = socket(AF_INET, SOCK_STREAM, 0);
    }
};
int main(int argc, char const *argv[])
{
    return 0;
}
