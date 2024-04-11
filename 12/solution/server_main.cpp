#include "server.h"

int main()
{
    Server server{3111};
    server.start();
}