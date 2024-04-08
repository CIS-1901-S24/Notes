#include "client.h"
#include "utils.h"

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>

using namespace std;

Client::Client()
{
}

void Client::connect_to(int port, const std::string& address)
{
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        fprintf(stderr, "Cannot open socket (%s)\n", strerror(errno));
        exit(1);
    }
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(port);
    inet_pton(AF_INET, address.data(), &(servaddr.sin_addr));
    int ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (ret < 0)
    {
        std::cerr << "unable to connect";
    }
}

void Client::run()
{
    Reader      reader(_fd);
    std::string message{};
    while (getline(std::cin, message))
    {
        bool success = send_data(_fd, message);
        std::cout << (success ? "success" : " fail");
        auto response = reader.receive_data();
        if (response.has_value())
        {
            std::cerr << response.value();
        }
        else
        {
            std::cerr << "connection closed\n";
            break;
        }
    }
}