//
//  NumericsTest.cpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 01/11/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include <stdio.h>

#include "catch.hpp"
#include "aot_simdlib.hpp"

using simdv = simd::recipes<4>;

// abs error: 9.53674e-07 i.e. error < 0.000`000`954
Approx Approx20bit(double value)
{
    return Approx(value).margin(1.0f/(1 << 20));
}

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
        
        float phase = 0;
        float phaseIncr = 0.07f;
        
        simdv::initPhase<20>(buffer.ptr(), phaseIncr);
        
        for (size_t i = 0; i < 20; ++i)
        {
            CHECK(buffer[i] == Approx(phase));
            phase = wrapPhase(phase + phaseIncr);
        }
    }

    SECTION("initPhaseWithStartValue")
    {
        simd::AlignedBuffer<20> buffer;
        
        float phase = 0.7;
        float phaseIncr = 0.07f;
        
        simdv::initPhase<20>(buffer.ptr(), phaseIncr, phase);
        
        for (size_t i = 0; i < 20; ++i)
        {
            CHECK(buffer[i] == Approx(phase));
            phase = wrapPhase(phase + phaseIncr);
        }
    }
    
    SECTION("loop")
    {
        LOOP_(0, 4)([&](size_t i)
        {
            CHECK(i <= 16 - simd::stride);
        });
        LOOP_(0, 8)([&](size_t i)
        {
            CHECK(i <= 16 - simd::stride);
        });
        LOOP_(0, 12)([&](size_t i)
        {
            CHECK(i <= 16 - simd::stride);
        });
        LOOP_(0, 16)([&](size_t i)
        {
            CHECK(i <= 16 - simd::stride);
        });
    }
    
    SECTION("updatePhase")
    {
        const size_t bufferSize = 16;
        float phaseIncr = 0.07f;
        float phase = 0.8f;
        
        simd::AlignedBuffer<bufferSize> buffer;

        simdv::initPhase<bufferSize>(buffer.ptr(), phaseIncr, phase);
        
        CHECK(phase == 0.8f);
        CHECK(phaseIncr == 0.07f);
        
        for (size_t round = 0; round < 20; ++round)
        {
            for (size_t i = 0; i < bufferSize; ++i)
            {
                CAPTURE(buffer[i]);
                CAPTURE(phase);
                CAPTURE((buffer[i] - phase) * 1e6);
                CAPTURE(round);
                CAPTURE(i);

                auto match = buffer[i] == Approx20bit(phase) || (std::abs(buffer[i]) == Approx(1) && buffer[i] == Approx(-phase));
                CHECK(match);
                phase = wrapPhase(phase + phaseIncr);
            }
            
            simdv::updatePhase<bufferSize>(buffer.ptr(), phaseIncr);
        }
    }
    
    SECTION("wrapPhase")
    {
        const size_t bufferSize = 32;
        simd::AlignedBuffer<bufferSize> buffer;

        float phaseIncr = (2.0f - -1.0f) / (bufferSize - 1);

        for (auto i = 0; i < bufferSize; ++i)
        {
            buffer[i] = -1 + i * phaseIncr;
        }
        
        CAPTURE(*std::min_element(buffer.data.begin(), buffer.data.end()));
        CAPTURE(*std::max_element(buffer.data.begin(), buffer.data.end()));

        simdv::wrapPhase<bufferSize>(buffer.ptr());

        float phase = -1;
        for (auto i = 0; i < bufferSize; ++i)
        {
            CHECK(buffer[i] == Approx20bit(phase));
            phase = wrapPhase(phase + phaseIncr);
        }
    }
    
    SECTION("fastSin")
    {
        const size_t bufferSize = 32;
        simd::AlignedBuffer<bufferSize * 2> buffer;
        float phaseIncr = 0.007f;
        
        for (float phase = -1; phase <= 2; )
        {
            for (size_t i = 0; i < bufferSize; ++i)
            {
                buffer[i] = phase += phaseIncr;
            }
            
            simdv::wrapPhase<bufferSize>(buffer.ptr());
            simdv::fastSin<bufferSize>(buffer.ptr(bufferSize/2), buffer.ptr());
            
            for (size_t i = 0; i < bufferSize; ++i)
            {
                CAPTURE(buffer[i]);
                CHECK(buffer[bufferSize/2 + i] == Approx20bit(std::sin(buffer[i] * simd::pi<float>)));
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
