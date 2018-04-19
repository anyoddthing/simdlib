//
//  SumBench.cpp
//  simdlib
//
//  Created by Daniel Doubleday on 18.04.18.
//  Copyright © 2018 Daniel Doubleday. All rights reserved.
//

#include "bench.hpp"

struct SumBench
{
    static SumBench instance;

    SumBench()
    {
        float random = 0;
        for (auto i = 0; i < buffer.size(); ++i)
        {
            random += buffer[i] = static_cast<float>(rand()) / RAND_MAX;
        }
    }
    
    NOINLINE baseLineTest(benchpress::context* ctx, float* result)
    {
        BENCH_ITER
        {
            float random = 0;
            for (auto i = 0; i < buffer.size(); ++i)
            {
                random += buffer[i];
            }
            *result = random;
        }
    }
    
    NOINLINE simdTest(benchpress::context* ctx, float* result)
    {
        BENCH_ITER
        {
            *result = simd::math::sum(buffer.data(), buffer.size());
        }
    }
    
    simd::AlignedBuffer<64> buffer;
};

SumBench SumBench::instance;
static float optkiller;

BENCHMARK("Buffer Sum BL", [](benchpress::context* ctx)
{
    SumBench::instance.baseLineTest(ctx, &optkiller);
})

BENCHMARK("Buffer Sum SIMD", [](benchpress::context* ctx)
{
    SumBench::instance.simdTest(ctx, &optkiller);
})


