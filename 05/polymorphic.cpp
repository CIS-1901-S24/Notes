#include <memory>
#include <iostream>

struct A
{
    int x;
    A(int x):x{x}
    {}

    virtual void f(){};
};

struct B : public A
{
    int y;
    B(int x, int y): A{x}, y{y}
    {}
};

int main()
{
    std::shared_ptr<A> a_ptr{std::make_shared<B>(10, 20)};
    std::cout << a_ptr->x << std::endl;
    // std::cout << a_ptr->y << std::endl; // Does not compile

    auto b_ptr = dynamic_cast<B*>(a_ptr.get());
    std::cout << b_ptr -> y << std::endl;
}