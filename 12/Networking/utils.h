#ifndef UTILS_H
#define UTILS_H

#include "arpa/inet.h"
#include "stdio.h"
#include "unistd.h"
#include <optional>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>

static constexpr unsigned BUFFER_LEN = 1024;

struct SocketException : public std::exception
{
    const char* what() const noexcept(true) override
    {
        return "Socket Read Error";
    }
};

class Reader
{
public:
    Reader(int sock);
    std::optional<std::string> receive_data() noexcept(false);

private:
    int                     _sock;
    std::queue<std::string> _queue;
    std::stringstream       _sstream;
    bool                    _carriage;
};

bool send_data(int sock, std::string& message);

#endif