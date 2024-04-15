#include <vector>
#include <string>
#include <benchmark/benchmark.h>


static const unsigned NUM_ITERATIONS = 16;
static const unsigned LEN = 1000000;

void f1()
{
    std::vector<std::string> v;
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        std::string s(LEN, 'A');
        v.push_back(s);
    } 
}

void f2()
{
    std::vector<std::string> v;
    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        v.emplace_back(LEN, 'A');
    } 
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
