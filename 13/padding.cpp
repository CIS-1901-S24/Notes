#include <iostream>

struct f1
{
    int x;
    double d;
    char c;
};

struct f2
{
    double d;
    int x;
    char c;
};

int main()
{
    std::cout << sizeof(f1) << std::endl; // prints 24
    std::cout << sizeof(f2) << std::endl; // prints 16
}