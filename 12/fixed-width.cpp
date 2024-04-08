#include <cstdint>
#include <iostream>

int main()
{
    uint16_t x = 1 << 17;     // will overflow
    uint32_t y = 5u - 10u;    // will overflow
    int64_t  z = -(1l << 62); // can be negative
    uint8_t  c = 'a';         // uint8_t is technically a char

    std::cout << x << " \n";
    std::cout << y << " \n";
    std::cout << z << "\n";
    std::cout << c << "\n"; // the operator is overloaded to print uint8_t as char
}