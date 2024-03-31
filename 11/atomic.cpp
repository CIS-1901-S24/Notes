#include <atomic>
#include <iostream>
#include <thread>

void f1(int& count)
{
    for (int i = 0; i < 100000; i++)
        count++;
}

void f2(std::atomic<int>& count)
{
    for (int i = 0; i < 100000; i++)
        count++;
}

int main()
{
    int              count        = 0;
    std::atomic<int> count_atomic = 0;

    std::thread t1(f1, std::ref(count));
    std::thread t2(f1, std::ref(count));

    std::thread t3(f2, std::ref(count_atomic));
    std::thread t4(f2, std::ref(count_atomic));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "count: " << count << std::endl;
    std::cout << "atomic count: " << count_atomic << std::endl;
}