#include "client.h"

int main()
{
    Client client{};
    client.connect_to(3111, "127.0.0.1");
    client.run();
}