#include <iostream>


template<typename T>
void f (T&& t)
{
    // t can be either lvalue or rvalue references
}

void foo(std::string& a, int& b)
{
    std::cout << "lvalue overload\n";
}

void foo(std::string&& a, int&& b)
{
    std::cout << "rvalue overload\n";
}

template <typename T1, typename T2>
void bar(T1&& a, T2&& b)
{
    foo(std::forward<T1>(a), std::forward<T2>(b));
}

int main()
{
    bar("hello",2);
    
    std::string s {"cis1901"};
    int i {10};
    
    bar(s,i);

    return 0;
}
