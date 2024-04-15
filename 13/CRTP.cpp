#include <iostream>
#include <benchmark/benchmark.h>
#include <vector>


class Animal
{
public:
    Animal(){}

    void virtual sound() = 0;
};

class Dog : public Animal
{
public:
    Dog() : Animal() {}
    void sound(){std::cerr << "bark";}
};

class Cat : public Animal
{
public:
    Cat() : Animal() {}
    void sound(){std::cerr << "meow";}
};

template<typename Derived>
class Animal2
{
public:
    Animal2(){}
    void sound(){ static_cast<Derived*>(this)->sound_impl();}
};

class Dog2 : public Animal2<Dog2>
{
public:
    Dog2 (){} 
    static void sound_impl(){std::cerr << "bark";} 
};

class Cat2 : public Animal2<Cat2>
{
public:
    Cat2 () {}
    static void sound_impl(){std::cerr << "mewo";} 
};

static void BM_CRTP(benchmark::State& state) {
  for (auto _ : state)
  {
    Dog2 d{};
    Cat2 c{};
    d.sound();
    c.sound();
  }
}

static void BM_Inheritance(benchmark::State& state) {
  for (auto _ : state)
  {
    Dog d{};
    Cat c{};
    d.sound();
    c.sound();
  }
}


BENCHMARK(BM_Inheritance);
BENCHMARK(BM_CRTP);

BENCHMARK_MAIN();