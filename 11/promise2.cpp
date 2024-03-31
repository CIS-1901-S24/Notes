#include <future>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

void producer(int i, std::promise<int> & p1, std::promise<int> & p2)
{
    p1.set_value(i + 1);
    p2.set_value(i + 2);
    std::this_thread::sleep_for(5s);
}

int main()
{
    std::promise<int> p1;
    std::future<int> result1 {p1.get_future()};

    std::promise<int> p2;
    std::future<int> result2 {p2.get_future()};

    std::thread t {producer, 12, std::ref(p1), std::ref(p2)};

    std::cout << result1.get() << std::endl;
    std::cout << result2.get() << std::endl;

    t.join();
}
