#ifndef Server_H
#define Server_H

#include "thread"
#include <array>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <vector>

class Server
{
public:
    Server(int);

    void start();

    ~Server();

private:
    int _port;
    int _listen_fd;

    void accept_connections();
    void handle_thread(int);
    void multicast(int, const std::string&);

    struct CCB
    {
        std::unique_ptr<std::thread> thread;
        int                          fd;
        int                          chat = -1;
        std::unique_ptr<std::mutex>  m;
    };
    std::vector<CCB>                 _connections;
    std::array<std::vector<int>, 10> _chats;
    std::shared_mutex                _connections_mutex;
    std::shared_mutex                _chats_mutex;
};

#endif