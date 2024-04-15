#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
#include <random>
#include <benchmark/benchmark.h>


static const unsigned LEN = 1024 * 1024;
void f1()
{
    std::vector<int> v{};
    for (int i = 0; i < LEN; i++)
        v.push_back(0);
}

void f2()
{
    std::list<int> l{};
    for (int i = 0; i < LEN; i++)
        l.push_back(0);
}

static void BM_F1(benchmark::State& state) {
  for (auto _ : state)
  {
   f1();
  }
}

static void BM_F2(benchmark::State& state) {
  for (auto _ : state)
  {
   f2();
  }
}


BENCHMARK(BM_F2);
BENCHMARK(BM_F1);

BENCHMARK_MAIN();
