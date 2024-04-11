#include "server.h"
#include "arpa/inet.h"
#include "utils.h"
#include <cctype>
#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>

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

        std::unique_lock lock(_connections_mutex);

        int i = _connections.size();
        _connections.emplace_back();
        auto& connection = _connections[i];

        connection.fd = comm_fd;

        auto thread       = std::make_unique<std::thread>(&Server::handle_thread, this, i);
        connection.thread = std::move(thread);
        auto m            = std::make_unique<std::mutex>();
        connection.m      = std::move(m);
    }
}

void Server::handle_thread(int i)
{
    int chat_id       = -1;
    int connection_fd = -1;
    {
        std::shared_lock lock(_connections_mutex);
        auto&            connection = _connections[i];
        connection_fd               = connection.fd;
        chat_id                     = connection.chat;
    }

    std::cout << connection_fd << std::endl;

    Reader reader(connection_fd);

    while (true)
    {
        {
            std::shared_lock lock(_connections_mutex);
            auto&            connection = _connections[i];
            chat_id                     = connection.chat;
        }

        try
        {
            const auto message = reader.receive_data();
            if (!message.has_value())
            {
                std::cerr << "connection closed\n";
                break;
            }

            auto val = message.value();

            if (val.size() > 5 && val.substr(0, 4) == "JOIN" && val[4] == ' ' &&
                std::isdigit(val[5]))
            {
                auto chat_idx = val[5] - '0';
                {
                    std::shared_lock l2{_connections_mutex};
                    std::unique_lock l1{_chats_mutex};
                    auto&            connection = _connections[i];
                    _chats[chat_idx].push_back(i);
                    connection.chat = chat_idx;
                }
                std::string resp{"joined chat #" + std::to_string(chat_idx) + "  succesfully"};
                send_data(connection_fd, resp);
                continue;
            }

            if (chat_id < 0 || chat_id >= 10)
            {
                std::string fallback_message = "Not in Chat";
                send_data(connection_fd, fallback_message);
            }
            else
            {
                multicast(chat_id, val);
            }
        }
        catch (const SocketException& e)
        {
            std::cerr << "error in connection\n";
        }
    }
}

void Server::multicast(int chat_id, const std::string& message)
{
    std::shared_lock l1{_chats_mutex};
    auto&            chat = _chats[chat_id];
    for (auto id : chat)
    {
        auto&            connection = _connections[id];
        std::scoped_lock lock{*(connection.m)};
        auto             cpy = message;
        send_data(connection.fd, cpy);
    }
}

Server::~Server()
{
    for (auto& ccb : _connections)
    {
        ccb.thread->join();
    }
}