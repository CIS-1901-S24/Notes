#ifndef Server_H
#define Server_H

#include "thread"

class Server
{
public:
    Server(int);

    void start();
    void accept_connections();

    ~Server();

private:
    int _port;
    int _listen_fd;
};

#endif