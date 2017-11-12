//
//  TestHelper.hpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 25/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#ifndef TestHelper_hpp
#define TestHelper_hpp

#include <stdio.h>
#include <array>

#include "catch.hpp"
#include "aot_simdlib.hpp"
#include "NoInliner.hpp"
#include "AlignedBuffer.hpp"

using namespace simd;

template <typename T>
struct TestHelper
{
    AlignedBuffer<64> buffer;
    using SimdType = typename SimdOps<T>::SimdType;
    
    void setBuffer(size_t start, float v1, float v2, float v3, float v4)
    {
        buffer.data[start + 0] = v1;
        buffer.data[start + 1] = v2;
        buffer.data[start + 2] = v3;
        buffer.data[start + 3] = v4;
    }
    
    float* data(size_t start)
    {
        return buffer.data.data() + start;
    }
    
    float valInMem(size_t index)
    {
        return buffer.data[index];
    }
    
    void testLoad()
    {
        setBuffer(0, 0.1, 0.2, 0.3, 0.4);
        SimdType v = SimdOps<T>::load(data(0));
        CHECK(v[0] == 0.1f);
        CHECK(v[1] == 0.2f);
        CHECK(v[2] == 0.3f);
        CHECK(v[3] == 0.4f);
    }
    
    void testStore()
    {
        SimdType v = { 0.4, 0.3, 0.2, 0.1 };
        SimdOps<T>::store(v, data(0));
        CHECK(valInMem(0) == 0.4f);
        CHECK(valInMem(1) == 0.3f);
        CHECK(valInMem(2) == 0.2f);
        CHECK(valInMem(3) == 0.1f);
    }
    
    void testAdd()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::add(v1, v2);
        CHECK(v[0] == Approx(0.2f));
        CHECK(v[1] == Approx(0.4f));
        CHECK(v[2] == Approx(0.6f));
        CHECK(v[3] == Approx(0.8f));
    }

    void testSubtract()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.1, 0.1, 0.1, 0.1 };
        SimdType v = SimdOps<T>::sub(v1, v2);
        CHECK(v[0] == Approx(0.0f));
        CHECK(v[1] == Approx(0.1f));
        CHECK(v[2] == Approx(0.2f));
        CHECK(v[3] == Approx(0.3f));
    }
    
    void testMultiply()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 1,   2,   3,   4 };
        SimdType v = SimdOps<T>::mul(v1, v2);
        CHECK(v[0] == Approx(0.1));
        CHECK(v[1] == Approx(0.4));
        CHECK(v[2] == Approx(0.9));
        CHECK(v[3] == Approx(1.6));
    }

    void testDevide()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 1,   2,   3,   4 };
        SimdType v = SimdOps<T>::div(v1, v2);
        CHECK(v[0] == Approx(0.1f));
        CHECK(v[1] == Approx(0.1f));
        CHECK(v[2] == Approx(0.1f));
        CHECK(v[3] == Approx(0.1f));
    }

    void testReciprocal()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::reciprocal(v1);
        
        CHECK(v[0] == Approx(1 / v1[0]));
        CHECK(v[1] == Approx(1 / v1[1]));
        CHECK(v[2] == Approx(1 / v1[2]));
        CHECK(v[3] == Approx(1 / v1[3]));
    }
    
    void testSqrt()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::sqrt(v1);
        
        CHECK(v[0] == Approx(std::sqrt(v1[0])));
        CHECK(v[1] == Approx(std::sqrt(v1[1])));
        CHECK(v[2] == Approx(std::sqrt(v1[2])));
        CHECK(v[3] == Approx(std::sqrt(v1[3])));
    }
    
    void testMin()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.9, 0.6, 0.1, -0.6 };
        
        SimdType v = SimdOps<T>::min(v1, v2);
        CHECK(v[0] == Approx(0.1));
        CHECK(v[1] == Approx(0.2));
        CHECK(v[2] == Approx(0.1));
        CHECK(v[3] == Approx(-0.6));
    }

    void testMax()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.9, 0.6, 0.1, -0.6 };
        
        SimdType v = SimdOps<T>::max(v1, v2);
        CHECK(v[0] == Approx(0.9));
        CHECK(v[1] == Approx(0.6));
        CHECK(v[2] == Approx(0.3));
        CHECK(v[3] == Approx(0.4));
    }
    
    void testCompareGT()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.9, 0.6, 0.1, -0.6 };
        
        SimdType v = SimdOps<T>::cmpgt(v1, v2);
        
        CHECK(SimdOps<T>::getIval(v, 0) == 0x00000000);
        CHECK(SimdOps<T>::getIval(v, 1) == 0x00000000);
        CHECK(SimdOps<T>::getIval(v, 2) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 3) == 0xffffffff);
    }

    void testCompareGTE()
    {
        SimdType v1 = { 0.1, 0.6, 0.3, 0.4 };
        SimdType v2 = { 0.9, 0.6, 0.1, -0.6 };
        
        SimdType v = SimdOps<T>::cmpge(v1, v2);
        CHECK(SimdOps<T>::getIval(v, 0) == 0x00000000);
        CHECK(SimdOps<T>::getIval(v, 1) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 2) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 3) == 0xffffffff);
    }

    void testCompareLT()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.9, 0.6, 0.1, -0.6 };
        
        SimdType v = SimdOps<T>::cmplt(v1, v2);
        CHECK(SimdOps<T>::getIval(v, 0) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 1) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 2) == 0x00000000);
        CHECK(SimdOps<T>::getIval(v, 3) == 0x00000000);
    }

    void testCompareLTE()
    {
        SimdType v1 = { 0.1, 0.2, 0.1, 0.4 };
        SimdType v2 = { 0.9, 0.6, 0.1, -0.6 };
        
        SimdType v = SimdOps<T>::cmple(v1, v2);
        CHECK(SimdOps<T>::getIval(v, 0) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 1) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 2) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 3) == 0x00000000);
    }

    void testComplement()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::complement(v1);
        
        CHECK(SimdOps<T>::getIval(v, 0) == ~SimdOps<T>::getIval(v1, 0));
        CHECK(SimdOps<T>::getIval(v, 1) == ~SimdOps<T>::getIval(v1, 1));
        CHECK(SimdOps<T>::getIval(v, 2) == ~SimdOps<T>::getIval(v1, 2));
        CHECK(SimdOps<T>::getIval(v, 3) == ~SimdOps<T>::getIval(v1, 3));
    }
    
    void testMaskXOr()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::maskxor(v1, v2);
        
        CHECK(v[0] == 0);
        CHECK(v[1] == 0);
        CHECK(v[2] == 0);
        CHECK(v[3] == 0);
        
        v2 = { 0.2, 0.3, 0.4, 0.5 };
        v = SimdOps<T>::maskxor(v1, v2);
        
        CHECK(SimdOps<T>::getIval(v, 0) == (SimdOps<T>::getIval(v1, 0) ^ SimdOps<T>::getIval(v2, 0)));
        CHECK(SimdOps<T>::getIval(v, 1) == (SimdOps<T>::getIval(v1, 1) ^ SimdOps<T>::getIval(v2, 1)));
        CHECK(SimdOps<T>::getIval(v, 2) == (SimdOps<T>::getIval(v1, 2) ^ SimdOps<T>::getIval(v2, 2)));
        CHECK(SimdOps<T>::getIval(v, 3) == (SimdOps<T>::getIval(v1, 3) ^ SimdOps<T>::getIval(v2, 3)));
    }
    
    void testMaskOr()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::maskor(v1, v2);
        
        CHECK(v[0] == 0.1f);
        CHECK(v[1] == 0.2f);
        CHECK(v[2] == 0.3f);
        CHECK(v[3] == 0.4f);
        
        v2 = { 0.2, 0.3, 0.4, 0.5 };
        v = SimdOps<T>::maskor(v1, v2);
        
        CHECK(SimdOps<T>::getIval(v, 0) == (SimdOps<T>::getIval(v1, 0) | SimdOps<T>::getIval(v2, 0)));
        CHECK(SimdOps<T>::getIval(v, 1) == (SimdOps<T>::getIval(v1, 1) | SimdOps<T>::getIval(v2, 1)));
        CHECK(SimdOps<T>::getIval(v, 2) == (SimdOps<T>::getIval(v1, 2) | SimdOps<T>::getIval(v2, 2)));
        CHECK(SimdOps<T>::getIval(v, 3) == (SimdOps<T>::getIval(v1, 3) | SimdOps<T>::getIval(v2, 3)));
    }
    
    void testMaskOrNot()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v = SimdOps<T>::maskornot(v1, v2);
        
        CHECK(SimdOps<T>::getIval(v, 0) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 1) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 2) == 0xffffffff);
        CHECK(SimdOps<T>::getIval(v, 3) == 0xffffffff);
        
        v2 = { 0.2, 0.3, 0.4, 0.5 };
        v = SimdOps<T>::maskornot(v1, v2);
        
        CHECK(SimdOps<T>::getIval(v, 0) == (SimdOps<T>::getIval(v1, 0) | ~SimdOps<T>::getIval(v2, 0)));
        CHECK(SimdOps<T>::getIval(v, 1) == (SimdOps<T>::getIval(v1, 1) | ~SimdOps<T>::getIval(v2, 1)));
        CHECK(SimdOps<T>::getIval(v, 2) == (SimdOps<T>::getIval(v1, 2) | ~SimdOps<T>::getIval(v2, 2)));
        CHECK(SimdOps<T>::getIval(v, 3) == (SimdOps<T>::getIval(v1, 3) | ~SimdOps<T>::getIval(v2, 3)));
    }
    
    void testMaskAnd()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2;
        SimdOps<T>::setIval(v2, 0, 0x00000000);
        SimdOps<T>::setIval(v2, 1, 0xffffffff);
        SimdOps<T>::setIval(v2, 2, 0x00000000);
        SimdOps<T>::setIval(v2, 3, 0xffffffff);
        
        SimdType v = SimdOps<T>::maskand(v1, v2);
        CHECK(v[0] == 0);
        CHECK(v[1] == Approx(0.2));
        CHECK(v[2] == 0);
        CHECK(v[3] == Approx(0.4));
    }
    
    void testMaskAndNot()
    {
        SimdType v1 = { 0.1, 0.2, 0.3, 0.4 };
        SimdType v2;
        SimdOps<T>::setIval(v2, 0, 0x00000000);
        SimdOps<T>::setIval(v2, 1, 0xffffffff);
        SimdOps<T>::setIval(v2, 2, 0x00000000);
        SimdOps<T>::setIval(v2, 3, 0xffffffff);
        
        SimdType v = SimdOps<T>::maskandnot(v1, v2);
        CHECK(v[0] == Approx(0.1));
        CHECK(v[1] == 0);
        CHECK(v[2] == Approx(0.3));
        CHECK(v[3] == 0);
    }

    void testSelect()
    {
        SimdType v1 = { 0.1, 0.1, 0.1, 0.1 };
        SimdType v2 = { 0.2, 0.2, 0.2, 0.2 };
        
        SimdType mask;
        SimdOps<T>::setIval(mask, 0, 0x00000000);
        SimdOps<T>::setIval(mask, 1, 0xffffffff);
        SimdOps<T>::setIval(mask, 2, 0x00000000);
        SimdOps<T>::setIval(mask, 3, 0xffffffff);
        
        SimdType v = SimdOps<T>::select(v1, v2, mask);
        CHECK(v[0] == Approx(0.2));
        CHECK(v[1] == Approx(0.1));
        CHECK(v[2] == Approx(0.2));
        CHECK(v[3] == Approx(0.1));
    }

};

#endif /* TestHelper_hpp */
