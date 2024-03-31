#include <future>
#include <iostream>
#include <thread>

using namespace std;

void producer(int i, promise<int>& p)
{
    p.set_value(i + 1);
}

int main()
{
    promise<int> p;
    future<int>  result{p.get_future()};

    jthread t{producer, 12, ref(p)};

    cout << result.get();
}
