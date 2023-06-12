#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

class Socket
{
    int server_fd, new_socket, valread;
    uint16_t port;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1];

public:
    Socket(uint16_t port) : port{port} {}
    void create();
    void close();
    bool read(char* c);
    void send(const char *msg);
};
