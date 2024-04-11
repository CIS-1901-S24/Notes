#ifndef CLIENT_H
#define CLIENT_H
#include "utils.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class Client
{
public:
    Client();
    void connect_to(int port, const std::string& address);
    void handle_read(int fd);
    void run();

private:
    int         _fd;
    std::string _address;
};

#endif