#include <iostream>

// base cases
void print(const std::string &s)
{
    std::cout << s << std::endl;
}

// we can have any types
void print(bool b)
{
    std::cout << (b ? "True" : "False") << std::endl;
}


template<typename T, typename... Args>
void print(T value, Args... args)
{
    print(value);
    print(args...);
}

int main()
{
    std::string s {"cis"};
    bool b = true;
    std::string c {"1901"};
    print(s, b, c);

    // try out different number / order of arguments
    // print(b, s, s, b, b, c);
}