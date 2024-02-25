#include <iostream>
#include <string>

using namespace std;

// CRTP
template<typename Derived>
class Animal
{
    
public:
    void pet()
    {
        // do some stuff ...
        static_cast<Derived*>(this)->sound();
        // do some stuff ...
    }
};

class Cat : public Animal<Cat>
{
public:
    void sound() 
    {
        std::cout << "meow\n";
    }
};

class Dog : public Animal<Dog>
{
public:
    void sound()
    {
        std::cout << "bark\n";
    }
};



// Virtual immplementation for reference
// class Animal2
// {
// public:
//   void virtual sound() = 0;
//   void pet()
//   {
//     // do some stuff ...
//     sound();
//     // do some stuff ...
//   }
// };

// class Cat2 : public Animal2
// {
// public:
//     void sound() override
//     {
//         std::cout << "meow\n";
//     }
// };

// class Dog2 : public Animal2
// {
// public:
//     void sound() override
//     {
//         std::cout << "bark\n";
//     }
// };




int main()
{

    //CRTP
    Dog* dog = new Dog{};
    dog->pet();
    
    // virtual implementation
    // Dog2* dog2 = new Dog2{};
    // dog2->pet();
    
    return 0;
}
