#include "server.h"
#include "arpa/inet.h"
#include "utils.h"
#include <iostream>

Server::Server(int port) : _port{port}
{
}

// lot of boilerplate code, dont worry about it
void Server::start()
{
    struct sockaddr_in address;
    if ((_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket Error: ");
        exit(EXIT_FAILURE);
    }

    int op  = 1;
    int ret = setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
    if (ret < 0)
    {
        perror("In setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons(_port);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(_listen_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("Bind Error: ");
        exit(EXIT_FAILURE);
    }

    if (listen(_listen_fd, 100) < 0)
    {
        perror("Listen Error: ");
        exit(EXIT_FAILURE);
    }

    accept_connections();
}

void Server::accept_connections()
{
    while (true)
    {
        struct sockaddr_in clientaddr;
        socklen_t          clientaddrlen = sizeof(clientaddr);
        int comm_fd = accept(_listen_fd, (struct sockaddr*)&clientaddr, &clientaddrlen);
        if (comm_fd < 0)
            continue;
        std::cerr << "accepted a new connection\n";

        Reader reader(comm_fd);

        while (true)
        {
            try
            {
                const auto message = reader.receive_data();
                if (!message.has_value())
                {
                    std::cerr << "connection closed\n";
                    break;
                }
                auto response = "echoing: " + message.value();
                std::cerr << "resp: " << response;
                send_data(comm_fd, response);
            }
            catch (const SocketException& e)
            {
                std::cerr << "error in connection\n";
            }
        }

        // TODO: Complete
    }
}

Server::~Server()
{
}