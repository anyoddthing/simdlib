//
//  simdlib_common.h
//  VectorialHost
//
//  Created by Daniel Doubleday on 24/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#pragma once

#include <stdint.h>
#include <cmath>
#include <array>
#include <functional>

namespace simd
{
    static inline float toFloat(uint val)
    {
        union {
            uint ui;
            float f;
        } cast = { val };
        return cast.f;
    }
    
    static inline uint toUInt(float val)
    {
        union {
            float f;
            uint ui;
        } cast = { val };
        return cast.ui;
    }
    
    namespace detail
    {
        static inline float eval(std::function<bool()> predicate)
        {
            uint val = predicate() ? 0xffffffff : 0x00000000;
            return toFloat(val);
        }
    }

    template <typename T>
    struct SimdOps
    {
        using SimdType = T;
        
        static uint getIval(const T& simd, size_t index)
        {
            return toUInt(simd[index]);
        }
        
        static void setIval(T& simd, size_t index, uint val)
        {
            simd[index] = toFloat(val);
        }
        
        static T create(float values[4])
        {
            return create( values[0], values[1], values[2], values[3] );
        }
        
        static T create(float v1, float v2, float v3, float v4)
        {
            T result { v1, v2, v3, v4 };
            return result;
        }
        
        static T create(float v)
        {
            T result { v, v, v, v };
            return result;
        }
        
        static T load(const float* src)
        {
            T v;
            v[0] = src[0];
            v[1] = src[1];
            v[2] = src[2];
            v[3] = src[3];
            return v;
        }
        
        static void store(const T& v, float* dest)
        {
            dest[0] = v[0];
            dest[1] = v[1];
            dest[2] = v[2];
            dest[3] = v[3];
        }
        
        static T add(const T& v1, const T& v2)
        {
            T result { v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2], v1[3] + v2[3] } ;
            return result;
        }
        
        static T sub(const T& v1, const T& v2)
        {
            T result { v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2], v1[3] - v2[3] } ;
            return result;
        }
        
        static T mul(const T& v1, const T& v2)
        {
            T result { v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2], v1[3] * v2[3] } ;
            return result;
        }

        static T div(const T& v1, const T& v2)
        {
            T result { v1[0] / v2[0], v1[1] / v2[1], v1[2] / v2[2], v1[3] / v2[3] } ;
            return result;
        }

        static T reciprocal(const T& v)
        {
            T result { 1 / v[0], 1 / v[1], 1 / v[2], 1 / v[3] } ;
            return result;
        }
        
        static T sqrt(const T& v)
        {
            T result { std::sqrt(v[0]), std::sqrt(v[1]), std::sqrt(v[2]), std::sqrt(v[3]) } ;
            return result;
        }
    
        static T min(const T& v1, const T& v2)
        {
            T result { std::min(v1[0], v2[0]), std::min(v1[1], v2[1]), std::min(v1[2], v2[2]), std::min(v1[3], v2[3]) } ;
            return result;
        }

        static T max(const T& v1, const T& v2)
        {
            T result { std::max(v1[0], v2[0]), std::max(v1[1], v2[1]), std::max(v1[2], v2[2]), std::max(v1[3], v2[3]) } ;
            return result;
        }

        static T cmpgt(const T& v1, const T& v2)
        {
            T result {
                detail::eval([&]() { return v1[0] > v2[0]; } ),
                detail::eval([&]() { return v1[1] > v2[1]; } ),
                detail::eval([&]() { return v1[2] > v2[2]; } ),
                detail::eval([&]() { return v1[3] > v2[3]; } ),
            };
            return result;
        }

        static T cmpge(const T& v1, const T& v2)
        {
            T result {
                detail::eval([&]() { return v1[0] >= v2[0]; } ),
                detail::eval([&]() { return v1[1] >= v2[1]; } ),
                detail::eval([&]() { return v1[2] >= v2[2]; } ),
                detail::eval([&]() { return v1[3] >= v2[3]; } ),
            };
            return result;
        }

        static T cmplt(const T& v1, const T& v2)
        {
            T result {
                detail::eval([&]() { return v1[0] < v2[0]; } ),
                detail::eval([&]() { return v1[1] < v2[1]; } ),
                detail::eval([&]() { return v1[2] < v2[2]; } ),
                detail::eval([&]() { return v1[3] < v2[3]; } ),
            };
            return result;
        }

        static T cmple(const T& v1, const T& v2)
        {
            T result {
                detail::eval([&]() { return v1[0] <= v2[0]; } ),
                detail::eval([&]() { return v1[1] <= v2[1]; } ),
                detail::eval([&]() { return v1[2] <= v2[2]; } ),
                detail::eval([&]() { return v1[3] <= v2[3]; } ),
            };
            return result;
        }
        
        static T complement(const T& v)
        {
            T result;
            setIval(result, 0, ~ getIval(v, 0));
            setIval(result, 1, ~ getIval(v, 1));
            setIval(result, 2, ~ getIval(v, 2));
            setIval(result, 3, ~ getIval(v, 3));
            return result;
        }

        static T maskxor(const T& v, const T& mask)
        {
            T result;
            setIval(result, 0, getIval(v, 0) ^ getIval(mask, 0));
            setIval(result, 1, getIval(v, 1) ^ getIval(mask, 1));
            setIval(result, 2, getIval(v, 2) ^ getIval(mask, 2));
            setIval(result, 3, getIval(v, 3) ^ getIval(mask, 3));
            return result;
        }
        
        static T maskor(const T& v1, const T& v2)
        {
            T result;
            setIval(result, 0, getIval(v1, 0) | getIval(v2, 0));
            setIval(result, 1, getIval(v1, 1) | getIval(v2, 1));
            setIval(result, 2, getIval(v1, 2) | getIval(v2, 2));
            setIval(result, 3, getIval(v1, 3) | getIval(v2, 3));
            return result;
        }

        static T maskornot(const T& v1, const T& v2)
        {
            T result;
            setIval(result, 0, getIval(v1, 0) | ~getIval(v2, 0));
            setIval(result, 1, getIval(v1, 1) | ~getIval(v2, 1));
            setIval(result, 2, getIval(v1, 2) | ~getIval(v2, 2));
            setIval(result, 3, getIval(v1, 3) | ~getIval(v2, 3));
            return result;
        }

        static T maskand(const T& v1, const T& v2)
        {
            T result;
            setIval(result, 0, getIval(v1, 0) & getIval(v2, 0));
            setIval(result, 1, getIval(v1, 1) & getIval(v2, 1));
            setIval(result, 2, getIval(v1, 2) & getIval(v2, 2));
            setIval(result, 3, getIval(v1, 3) & getIval(v2, 3));
            return result;
        }

        static T maskandnot(const T& v1, const T& v2)
        {
            T result;
            setIval(result, 0, getIval(v1, 0) & ~getIval(v2, 0));
            setIval(result, 1, getIval(v1, 1) & ~getIval(v2, 1));
            setIval(result, 2, getIval(v1, 2) & ~getIval(v2, 2));
            setIval(result, 3, getIval(v1, 3) & ~getIval(v2, 3));
            return result;
        }
        
        static T select(const T& v1, const T& v2, const T& mask)
        {
            return maskor(maskand(v1, mask), maskandnot(v2, mask));
        }

        static float sum(const T& v)
        {
            return v[0] + v[1] + v[2] + v[3];
        }
    };

}


