//
//  SimdVec.hpp
//  simdlib
//
//  Created by Daniel Doubleday on 11/11/17.
//  Copyright © 2017 Daniel Doubleday. All rights reserved.
//

#pragma once

namespace simd
{

template <typename T>
struct SimdT
{
public:
    
    using ValueType = T;
    
//    float& operator[]       (const size_t index)       { return val[index]; }
    const float operator[] (const size_t index) const { return val[index]; }

    SimdT() {}
    
    SimdT(T val) : val(val) {}
    
    SimdT(const float *src)
    {
        val = SimdOps<T>::load(src);
    }

    SimdT(float v1, float v2, float v3, float v4)
    {
        val = SimdOps<T>::create(v1, v2, v3, v4);
    }
    
    SimdT(float v)
    {
        val = SimdOps<T>::create(v);
    }
    
    void load(const float* src)
    {
        val = SimdOps<T>::load(src);
    }
    
    void store(float *dest) const
    {
        SimdOps<T>::store(val, dest);
    }
    
    const SimdT operator + (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::add(val, other.val) };
    }
    
    SimdT& operator += (const SimdT &other)
    {
        val = SimdOps<T>::add(val, other.val);
        return *this;
    }
    
    const SimdT operator - (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::sub(val, other.val) };
    }
    
    SimdT& operator -= (const SimdT &other)
    {
        val = SimdOps<T>::sub(val, other.val);
    }
    
    const SimdT operator * (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::mul(val, other.val) };
    }
    
    SimdT& operator *= (const SimdT &other)
    {
        val = SimdOps<T>::mul(val, other.val);
        return *this;
    }
    const SimdT operator / (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::div(val, other.val) };
    }
    
    const SimdT operator < (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::cmplt(val, other.val) };
    }
    
    const SimdT operator <= (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::cmple(val, other.val) };
    }

    const SimdT operator > (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::cmpgt(val, other.val) };
    }
    
    const SimdT operator >= (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::cmpge(val, other.val) };
    }
    
    const SimdT operator | (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::maskor(val, other.val) };
    }

    const SimdT operator & (const SimdT &other) const
    {
        return SimdT { SimdOps<T>::maskand(val, other.val) };
    }
    
    SimdT select(const SimdT &val1, const SimdT &val2) const
    {
        return SimdT
        {
            SimdOps<T>::select(val1.val, val2.val, val)
        };
    }
    
    float sum()
    {
        return SimdOps<T>::sum(val);
    }
    
    T val;
};

using Vec = SimdT<simd_t>;

}
