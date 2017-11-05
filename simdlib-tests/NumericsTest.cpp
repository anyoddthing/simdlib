//
//  NumericsTest.cpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 01/11/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include <stdio.h>

#include "catch.hpp"
#include "Numerics.hpp"
#include "AlignedBuffer.hpp"

using simdv = simd::recipes<4>;

float wrapPhase(float val)
{
    return (val > 1.0f) ? val - 2.0f : val;
}

TEST_CASE( "Numrics Test" )
{
    SECTION("memset")
    {
        simd::AlignedBuffer<16> buffer;
        simdv::memset<16>(buffer.ptr(), 0.3f);
        for (size_t i = 0; i < buffer.size(); ++i)
        {
            CHECK(buffer[i] == Approx(0.3));
        }
    }
    
    SECTION("memcopy")
    {
        simd::AlignedBuffer<8> buffer { 0.0f, 0.1f, 0.2f, 0.3f };
        simdv::memcopy<4>(buffer.ptr(4), buffer.ptr());
        for (size_t i = 0; i < 4; ++i)
        {
            CHECK(buffer[i] == Approx(0.1 * i));
            CHECK(buffer[i + 4] == Approx(0.1 * i));
        }
    }
    
    SECTION("add")
    {
        simd::AlignedBuffer<3 * 8> buffer;
        // src1
        buffer.set(0 * 4, 0.1, 0.2, 0.3, 0.4);
        buffer.set(1 * 4, 0.5, 0.6, 0.7, 0.8);
        // src2
        buffer.set(2 * 4, 0.1, 0.2, 0.3, 0.4);
        buffer.set(3 * 4, 0.5, 0.6, 0.7, 0.8);
        
        simdv::add<8>(buffer.ptr(2 * 8), buffer.ptr(0), buffer.ptr(8));
        
        for (size_t i = 0; i < 8; ++i)
        {
            CHECK(buffer[2 * 8 + i] == Approx(2 * ((i / 10.0f) + 0.1f)));
        }
    }

    SECTION("initPhase")
    {
        simd::AlignedBuffer<20> buffer;
        simdv::initPhase<20>(buffer.ptr(), 0.01);
        
        float phase = 0;
        for (size_t i = 0; i < 20; ++i)
        {
            CHECK(buffer[i] == Approx(phase));
            phase += 0.01;
        }
    }
    
    SECTION("updatePhase")
    {
        simd::AlignedBuffer<8> buffer;
        simdv::initPhase<8>(buffer.ptr(), 0.1f);
        
        float phase = 0.8f;
        for (size_t round = 0; round < 10; ++round)
        {
            simdv::updatePhase<8>(buffer.ptr(), 0.1f);
            
            for (size_t i = 0; i < 8; ++i)
            {
                CAPTURE(buffer[i]);
                CAPTURE(phase);

                auto match = buffer[i] == Approx(phase).scale(0.01) || (std::abs(buffer[i]) == Approx(1) && buffer[i] == Approx(-phase));
                CHECK(match);
                phase = wrapPhase(phase + 0.1);
            }
        }
    }
    
    SECTION("fastSin")
    {
        simd::AlignedBuffer<8> buffer;
        float phaseIncr = 0.05f;
        for (float phase = -1; phase <= 1; )
        {
            for (size_t i = 0; i < 4; ++i)
            {
                buffer[i] = phase += phaseIncr;
            }
            
            simdv::fastSin<4>(buffer.ptr(4), buffer.ptr());
            
            for (size_t i = 0; i < 4; ++i)
            {
                CHECK(buffer[4 + i] == Approx(std::sin(buffer[i] * simd::pi<float>)).scale(0.01));
            }
        }
    }
    
    SECTION("fastSinAmp")
    {
        simd::AlignedBuffer<16> buffer;
        float phaseIncr = 0.05f;
        for (float phase = -1; phase <= 1; )
        {
            // amp
            for (size_t i = 0; i < 4; ++i)
            {
                buffer[i] = 0.5 + (i * 0.1);
            }
            
            // phase
            for (size_t i = 0; i < 4; ++i)
            {
                buffer[i + 4] = phase += phaseIncr;
            }
            
            simdv::fastSin<4, true>(buffer.ptr(8), buffer.ptr(4), buffer.ptr(0));
            
            for (size_t i = 0; i < 4; ++i)
            {
                CHECK(buffer[8 + i] == Approx(std::sin(buffer[i + 4] * simd::pi<float>) * buffer[i]).scale(0.01));
            }
        }
    }
}
