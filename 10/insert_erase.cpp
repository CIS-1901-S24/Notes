#include <vector>
#include <deque>
#include <iostream>

int main()
{
    std::vector v = {1,2,3,4,5,6};
    auto it = v.begin() + 2;
    v.erase(it); // removes the thrid element
    for (auto num : v)
    {
        std::cout << num << std::endl;
    }


    std::cout <<"-----------\n";


    std::deque<int> d{1,3,4,5,6};
    auto it2 = d.begin() + 1;
    d.insert(it2, 2); // add 2 before the second element
    for (auto num: d)
    {
        std::cout << num << std::endl;
    }
}