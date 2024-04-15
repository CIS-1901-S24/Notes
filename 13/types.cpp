#include <cstddef>
#include <numeric>
#include <iostream>
#include <vector>
#include <benchmark/benchmark.h>

static const unsigned LEN = 1024 * 1024;


int f1()
{

    std::vector<int> v(LEN, 10);
    int sum = 0;
    for (auto x : v)
        sum += x;

    return sum;
}

uint8_t f2()
{

    std::vector<uint8_t> v(LEN, 10);
    uint8_t sum = 0;
    for (auto x : v)
        sum += x;

    return sum;
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