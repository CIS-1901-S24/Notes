#include <ranges>
#include <vector>
#include <set>
#include <iostream>

int main()
{
    std::vector<int> v1 = {5,4,3,2,1};
    std::set<int> s1 = {5,4,3,2,1};

    std::sort(v1.begin(), v1.end());
    //std::sort(s1.begin(), s1.end());

    for (auto num : v1)
    {
        std::cout << num << std::endl;
    }


    std::vector<int> v2 = {5,4,3,2,1};
    std::set<int> s2 = {5,4,3,2,1};

    std::ranges::sort(v2);
    //std::ranges::sort(s2);

    for (auto num : v2)
    {
        std::cout << num << std::endl;
    }

}