//
//  SimdTTest.cpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 25/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "Simd.hpp"
#include "TestHelper.hpp"

using namespace simd;

TEST_CASE( "SimdT type tests" )
{

    SECTION("Initialization")
    {
        Vec v { 0.1, 0.2, 0.3, 0.4 };
        CHECK(v[0] == 0.1f);
        CHECK(v[1] == 0.2f);
        CHECK(v[2] == 0.3f);
        CHECK(v[3] == 0.4f);
    }
    
    SECTION("Load")
    {
        AlignedBuffer<4> buffer;
        buffer.set(0, 0.1, 0.2, 0.3, 0.4);
        
        Vec v;
        v.load(buffer.ptr(0));
        CHECK(v[0] == 0.1f);
        CHECK(v[1] == 0.2f);
        CHECK(v[2] == 0.3f);
        CHECK(v[3] == 0.4f);        
    }
    
    SECTION("Store")
    {
        AlignedBuffer<4> buffer;
        Vec v { 0.1, 0.2, 0.3, 0.4 };
        v.store(buffer.ptr(0));
        
        CHECK(buffer[0] == 0.1f);
        CHECK(buffer[1] == 0.2f);
        CHECK(buffer[2] == 0.3f);
        CHECK(buffer[3] == 0.4f);        
    }
    
    SECTION("Add")
    {
        Vec v1 { 0.1, 0.2, 0.3, 0.4 };
        Vec v2 { 0.1, 0.2, 0.3, 0.4 };
        
        auto v = v1 + v2;
        CHECK(v[0] == Approx(2 * 0.1f));
        CHECK(v[1] == Approx(2 * 0.2f));
        CHECK(v[2] == Approx(2 * 0.3f));
        CHECK(v[3] == Approx(2 * 0.4f));
    }

    SECTION("Subtract")
    {
        Vec v1 { 0.2, 0.3, 0.4, 0.5 };
        Vec v2 { 0.1, 0.2, 0.3, 0.4 };
        
        auto v = v1 - v2;
        CHECK(v[0] == Approx(0.1f));
        CHECK(v[1] == Approx(0.1f));
        CHECK(v[2] == Approx(0.1f));
        CHECK(v[3] == Approx(0.1f));
    }

}
