#include <iostream>
#include <list>
#include <queue>

std::list<int> mergeSortedLists(const std::list<std::list<int>>& lists)
{
    std::list<int> result{};
    using pair_t = std::pair<std::list<int>::const_iterator, std::list<int>::const_iterator>;

    auto comp = [](const pair_t& p1, const pair_t& p2) { return *(p1.first) > *(p2.first); };

    std::priority_queue<pair_t, std::vector<pair_t>, decltype(comp)> pq{comp};

    for (const auto& list : lists)
    {
        pq.emplace(list.begin(), list.end());
    }

    while (!pq.empty())
    {
        auto pair = pq.top();
        pq.pop();

        result.emplace_back(*pair.first);

        pair.first++;
        if (pair.first != pair.second)
        {
            pq.push(pair);
        }
    }

    return result;
}

template <typename Container>
Container::value_type merge_templated(const Container& lists)
{
    using single_list    = Container::value_type;
    using const_iterator = Container::value_type::const_iterator;
    single_list result{};
    using pair_t = std::pair<const_iterator, const_iterator>;

    auto comp = [](const pair_t& p1, const pair_t& p2) { return *(p1.first) > *(p2.first); };

    std::priority_queue<pair_t, std::vector<pair_t>, decltype(comp)> pq{comp};

    for (const auto& list : lists)
    {
        pq.emplace(list.begin(), list.end());
    }

    while (!pq.empty())
    {
        auto pair = pq.top();
        pq.pop();

        result.emplace_back(*pair.first);

        pair.first++;
        if (pair.first != pair.second)
        {
            pq.push(pair);
        }
    }

    return result;
}

int main()
{
    std::vector<int> v1 = {1, 4, 7};
    std::vector<int> v2 = {2, 5, 8};
    std::vector<int> v3 = {3, 6, 9};

    std::vector<std::vector<int>> l = {v1, v2, v3};

    auto merged = merge_templated(l);

    for (const auto& v : merged)
    {
        std::cout << v << std::endl;
    }
}