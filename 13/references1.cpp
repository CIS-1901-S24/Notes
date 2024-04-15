#include <benchmark/benchmark.h>
#include <vector>
#include <string>


int f1 (std::vector<std::string> v)
{
    unsigned sum = 0;
    for (auto str : v)
    {
        sum += str.size();
    }

    return sum;
}

int f2 (const std::vector<std::string> &v)
{
    unsigned sum = 0;
    for (const auto &str : v)
    {
        sum += str.size();
    }

    return sum;
}

static void BM_F1(benchmark::State& state) {
  std::string s(10000, 'A');
  std::vector<std::string> v(10000, s);
  for (auto _ : state)
  {
   f1(v);
  }
}

static void BM_F2(benchmark::State& state) {
  std::string s(10000, 'A');
  std::vector<std::string> v(10000, s);
  for (auto _ : state)
  {
   f2(v);
  }
}


BENCHMARK(BM_F1);
BENCHMARK(BM_F2);

BENCHMARK_MAIN();


