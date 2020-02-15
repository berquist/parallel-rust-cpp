#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <random>

#include <benchmark/benchmark.h>

// http://ppc.cs.aalto.fi/ch2/

void step(float* r, const float* d, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            float v = std::numeric_limits<float>::infinity();
            for (size_t k = 0; k < n; ++k) {
                float x = d[n*i + k];
                float y = d[n*k + j];
                float z = x + y;
                v = std::min(v, z);
            }
            r[n*i + j] = v;
        }
    }
}

int main(int argc, char *argv[]) {
    constexpr size_t n = 3;
    const float d[n*n] = {
        0, 8, 2,
        1, 0, 9,
        4, 5, 0,
    };
    float r[n*n];
    step(r, d, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            std::cout << r[i*n + j] << " ";
        }
        std::cout << "\n";
    }
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}

static void BM_bench_500(benchmark::State& state) {
    constexpr size_t n = 500;
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dis(0.0, 1.0);
    float d[n*n];
    for (size_t i = 0; i < n*n; i++) {
        d[i] = dis(gen);
    }
    float r[n*n];
    for (auto _ : state) {
        step(r, d, n);
    }
}

BENCHMARK(BM_bench_500);
