//
//  SinGeneratorBench.cpp
//  simdlib
//
//  Created by Daniel Doubleday on 07/11/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include "bench.hpp"

using simdv = simd::recipes<8>;
static simd::AlignedBuffer<16> buffer { 0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07 };

struct SinGeneratorBench
{
    template <typename T>
    static T fastSin(T currentPhase) {
        // Factorial constants so code is easier to read.
        T IF3  = static_cast<T>(1.0) / (2 * 3);
        T IF5  = IF3 / (4 * 5);
        T IF7  = IF5 / (6 * 7);
        T IF9  = IF7 / (8 * 9);
        T IF11 = IF9 / (10 * 11);

        /* Wrap phase back into region where results are more accurate. */
        T yp = (currentPhase > static_cast<T>(0.5))
            ? static_cast<T>(1.0) - currentPhase
            : ((currentPhase < (static_cast<T>(-0.5)))
                ? (static_cast<T>(-1.0)) - currentPhase
                : currentPhase);

        T x = yp * simd::pi<T>;
        T x2 = (x * x);
        /* Taylor expansion out to x**11/11! factored into multiply-adds */
        
        auto v1 = x2 * -IF11 + IF9;
        auto v2 = x2 * v1 - IF7;
        auto v3 = x2 * v2 + IF5;
        auto v4 = x2 * v3 - IF3;
        auto v5 = x2 * v4 + 1;
        
        T fastsin = x * v5;
        return fastsin;
    }
    
    template <size_t Size>
    static void generateSinBaseLine(float *dest, float *phase)
    {
        for (size_t i = 0; i < Size; ++i)
        {
            dest[i] = fastSin<float>(phase[i]);
        }
    }
    

    NOINLINE static baseLineTest(benchpress::context* ctx)
    {
        for (size_t i = 0; i < ctx->num_iterations(); ++i)
        {
            generateSinBaseLine<8>(buffer.data() + 8, buffer.data());
        }
    }
    
    NOINLINE static simdTest(benchpress::context* ctx)
    {
        using simds = simd::recipes<8>;

        for (size_t i = 0; i < ctx->num_iterations(); ++i)
        {            
            simds::fastSin(buffer.data() + 8, buffer.data());
        }
    }
};

BENCHMARK("SinGenerator BL", [](benchpress::context* ctx)
{
    SinGeneratorBench::baseLineTest(ctx);
})

BENCHMARK("SinGenerator SIMD", [](benchpress::context* ctx)
{
    SinGeneratorBench::simdTest(ctx);
})
