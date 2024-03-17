#include <functional>
#include <iostream>
#include <memory>
#include <random>
 
int g(int n1)
{
    return n1;
}

struct Foo
{
    void print_sum(int n1, int n2)
    {
        std::cout << n1 + n2 << '\n';
    }
 
    int data = 10;
};

void f(int n1, int n2, int n3, int n4, int n5)
{
    std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}
 
int main()
{
    using namespace std::placeholders;  // for _1, _2, _3...
 
    auto f1 = std::bind(f, _1, _3, _2, 40, 50);
    f1(10, 20, 30); // makes a call to f(10, 20, 30, 40, 50)
 
    // nested bind
    auto f2 = std::bind(f, _2, std::bind(g, _1), _3, 40, 50);
    f2(20, 10, 30);
 
    // you can bind to a pointer to member function
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    f3(5);
 
    // you can point to a data member
    auto f5 = std::bind(&Foo::data, _1);
    std::cout << f5(foo) << '\n';

}