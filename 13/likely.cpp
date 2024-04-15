#include <iostream>

int main()
{
    std::cout << "divider program. enter two number to divide\n";
    int x, y;
    std::cin >> x >> y; 
    if (y != 0) [[likely]]
        std::cout << y/x;
    else
        std::cout << "cannot divide by zero";
}