#include <iostream>

__attribute__((always_inline))
int times2(int x)
{
    return 2*x;
}

__attribute__((noinline))
double square_root(double x)
{
    // complex logic
    return x;
}

int main()
{
    int y = 10;
    std::cout << times2(y);
}