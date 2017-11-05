//
//  NoInliner.cpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 25/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include "catch.hpp"
#include "Simd.hpp"
#include "NoInliner.hpp"
#include "TestHelper.hpp"
#include "Numerics.hpp"

#define NO_INLINER __attribute__ ((noinline)) NoInliner

using namespace simd;
using simdv = simd::recipes<4>;

void NO_INLINER::doSomeMath(float *input, float *output)
{
    Vec v1(input);
    Vec v2(0.3f);
    
    Vec result = v1 & v2;
    result.store(output);
}

void NO_INLINER::testDevision(float *input, float *output)
{
    Vec v1(input);
    Vec v2(0.3f);
    
    Vec result = v1 / v2;
    result.store(output);
}

void NO_INLINER::doSomeNumerics(float* output, const float* src, const float phaseIncrement)
{
    simdv::updatePhase(output, phaseIncrement);
}

template <size_t Size>
void NO_INLINER::testCrossFunctionCallOptimization(float* output, const float* src1, const float* src2, const float* src3)
{
    simdv::add(output, src1, src2);
    simdv::add(output, output, src3);
}

TEST_CASE("asm check")
{
    const int size = 4;
    AlignedBuffer<4 * size> buffer;
    NoInliner noinliner;
    
    noinliner.testCrossFunctionCallOptimization<size>(buffer.ptr(0), buffer.ptr(size), buffer.ptr(2 * size), buffer.ptr(3 * size));
}
