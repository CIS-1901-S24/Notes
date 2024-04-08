#include "utils.h"
#include <iostream>
#include <optional>
#include <sys/errno.h>

Reader::Reader(int sock) : _sock{sock}
{
}

std::optional<std::string> Reader::receive_data()
{
    while (_queue.empty())
    {
        char buffer[BUFFER_LEN];
        int  len_read = read(_sock, buffer, BUFFER_LEN);
        if (len_read == 0)
            return std::nullopt;
        if (len_read < 0)
            throw SocketException{};
        for (int i = 0; i < len_read; i++)
        {
            if (buffer[i] == '\n')
            {
                if (!_carriage)
                {
                    continue;
                }

                std::string res{_sstream.str()};
                _queue.push(res);
                _sstream.str("");
                _carriage = false;
            }
            if (buffer[i] == '\r')
            {
                _carriage = true;
            }
            if (buffer[i] != '\n' && buffer[i] != '\r')
            {
                _sstream << buffer[i];
            }
        }
    }

    auto ret = _queue.front();
    _queue.pop();
    return ret;
}

bool send_data(int sock, std::string& message)
{
    message += "\r\n";
    char buffer[BUFFER_LEN];
    strcpy(buffer, message.c_str());
    int len  = message.size();
    int sent = 0;
    while (sent < len)
    {
        int n = write(sock, &buffer[sent], len - sent);
        if (n < 0)
            return false;
        sent += n;
    }
    return true;
}