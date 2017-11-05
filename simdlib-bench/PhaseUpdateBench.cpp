//
//  ScratchBench.cpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 31/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include "benchpress/benchpress.hpp"

#include <chrono>
#include <iostream>
#include <array>
#include "AlignedBuffer.hpp"
#include "Simd.hpp"
#include "Numerics.hpp"

#define NOINLINE __attribute__ ((noinline)) static void

static simd::AlignedBuffer<8> buffer {0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07};
using simdv = simd::recipes<8>;

void phaseUpdateBaseLine(float *dest)
{
    for (size_t i = 0; i < 8; ++i)
    {
        float phase = dest[i] + 0.08;
        phase = (phase > 1.0f) ? phase - 2.0f : phase;
        dest[i] = phase;
    }
}

struct PhaseUpdateBench
{
    NOINLINE baseLineTest(benchpress::context* ctx)
    {
        for (size_t i = 0; i < ctx->num_iterations(); ++i)
        {
            phaseUpdateBaseLine(buffer.ptr());
        }
    }

    NOINLINE simdTest(benchpress::context* ctx)
    {
        for (size_t i = 0; i < ctx->num_iterations(); ++i)
        {
            simdv::updatePhase<8>(buffer.ptr(), 0.01f);
        }
    }
};


BENCHMARK("PhaseUpdate BL", [](benchpress::context* ctx)
{
    PhaseUpdateBench::baseLineTest(ctx);
})

BENCHMARK("PhaseUpdate SIMD", [](benchpress::context* ctx)
{
    PhaseUpdateBench::simdTest(ctx);
})
