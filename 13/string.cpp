#include <iostream>
#include <string>
#include <benchmark/benchmark.h>

std::string f1(unsigned len)
{
    std::string random_string;
    random_string.reserve(len);
    for (int i = 0 ; i < len; i++)
        random_string.push_back('A');
    return random_string;
}

void f2(std::string& random_string, unsigned len)
{
    random_string.clear();
    random_string.reserve(len);
    for (int i = 0 ; i < len; i++)
        random_string.push_back('A');
}

static const unsigned NUM_ITERATIONS = 16;
static const unsigned len = 1024 * 1024 * 128;

static void BM_F1(benchmark::State& state) {
  for (auto _ : state)
  {
   for (int i = 0; i < NUM_ITERATIONS; i++)
   {
    std::cerr << f1(len);
   }
  }
}

static void BM_F2(benchmark::State& state) {
  for (auto _ : state)
  {
   std::string s;
   for (int i = 0; i < NUM_ITERATIONS; i++)
   {
    f2(s, len);
    std::cerr << s; 
   }
  }
}


BENCHMARK(BM_F2);
BENCHMARK(BM_F1);

BENCHMARK_MAIN();

