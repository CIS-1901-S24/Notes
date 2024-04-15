#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <benchmark/benchmark.h>

int binarySearch(const std::vector<int>& array, int key, bool prefetch) {
    int low = 0, high = array.size() - 1, mid;
    while (low <= high) {
        mid = (low + high) / 2;
        if (prefetch)
        {
            __builtin_prefetch(&array[(mid + 1 + high) / 2], 0, 1);
            __builtin_prefetch(&array[(low + mid - 1) / 2], 0, 1);
        }

        if (array[mid] < key)
            low = mid + 1;
        else if (array[mid] == key)
            return mid;
        else if (array[mid] > key)
            high = mid - 1;
    }
    return -1;
}

static void BM_Prefetch(benchmark::State& state) {
    const int SIZE = 1024 * 1024 * 512;
    std::vector<int> array(SIZE);
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }

    const int NUM_LOOKUPS = 1024 * 1024 * 8;
    std::vector<int> lookups(NUM_LOOKUPS);
    std::srand(std::time(nullptr));
    for (int i = 0; i < NUM_LOOKUPS; i++) {
        lookups[i] = std::rand() % SIZE;
    }

    for (auto _ : state)
    {
        for (int i = 0; i < NUM_LOOKUPS; i++) {
            int result = binarySearch(array, lookups[i], true);
        }
    }
}

static void BM_No_Prefetch(benchmark::State& state) {
  const int SIZE = 1024 * 1024 * 512;
    std::vector<int> array(SIZE);
    for (int i = 0; i < SIZE; i++) {
        array[i] = i;
    }

    const int NUM_LOOKUPS = 1024 * 1024 * 8;
    std::vector<int> lookups(NUM_LOOKUPS);
    std::srand(std::time(nullptr));
    for (int i = 0; i < NUM_LOOKUPS; i++) {
        lookups[i] = std::rand() % SIZE;
    }

    for (auto _ : state)
    {
        for (int i = 0; i < NUM_LOOKUPS; i++) {
            int result = binarySearch(array, lookups[i], false);
        }
    }
}

BENCHMARK(BM_Prefetch);
BENCHMARK(BM_No_Prefetch);

BENCHMARK_MAIN();
