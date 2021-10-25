#include <benchmark/benchmark.h>

static void SomeFunctionToBenchmark(benchmark::State& state)
{
    for (auto _ : state)
    {

    }
}

BENCHMARK(SomeFunctionToBenchmark)->Iterations(1);

BENCHMARK_MAIN();