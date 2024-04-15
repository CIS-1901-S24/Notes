#include <benchmark/benchmark.h>
#include <unordered_map>
#include <string>






using Map = std::unordered_map<std::string, std::unordered_map<std::string, int>>;
Map m = {
        {"row1", {{"col1", 10}, {"col2", 20}}},
        {"row2", {{"col1", 30}, {"col2", 40}}},
    };


void modify1 (Map& m, std::string col, std::string row)
{
    if (m[col][row] != 0)
    {
        m[col][row] = 2*m[col][row];
    }
}

void modify2 (Map& m, std::string col, std::string row)
{
    auto& ref = m[col][row];
    if (ref != 0)
    {
        ref = 2*ref;
    }
}

static void BM_Modify1(benchmark::State& state) {
  for (auto _ : state)
  {
    modify1(m, "row1", "col1");
  }
}

static void BM_Modify2(benchmark::State& state) {
  for (auto _ : state)
  {
    modify2(m, "row1", "col1");
  }
}
BENCHMARK(BM_Modify1);
BENCHMARK(BM_Modify2);

BENCHMARK_MAIN();


