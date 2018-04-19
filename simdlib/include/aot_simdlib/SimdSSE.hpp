
//
//  simdlib_sse.h
//  VectorialHost
//
//  Created by Daniel Doubleday on 24/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#pragma once

#include <x86intrin.h>
//#include <xmmintrin.h>

namespace simd
{
    using simd_t = __m128;
    
    template <>
    struct SimdOps<__m128>
    {
        using SimdType = __m128;;
        
        static uint getIval(const SimdType& simd, size_t index)
        {
            return toUInt(simd[index]);
        }
        
        static void setIval(SimdType& simd, size_t index, uint val)
        {
            simd[index] = toFloat(val);
        }        
        
        static SimdType create(float v1, float v2, float v3, float v4)
        {
            return _mm_set_ps( v4, v3, v2, v1 );
        }
        
        static SimdType create(float v)
        {
            return _mm_set1_ps( v );
        }
        
        static SimdType load(const float* src)
        {
            return _mm_load_ps(src);
        }
        
        static void store(const SimdType& v, float* dest)
        {
            _mm_store_ps(dest, v);
        }
        
        static SimdType add(const SimdType& v1, const SimdType& v2)
        {
            return _mm_add_ps(v1, v2);
        }
        
        static SimdType sub(const SimdType& v1, const SimdType& v2)
        {
            return _mm_sub_ps(v1, v2);
        }
        
        static SimdType mul(const SimdType& v1, const SimdType& v2)
        {
            return _mm_mul_ps(v1, v2);
        }

        static SimdType div(const SimdType& v1, const SimdType& v2)
        {
            return _mm_div_ps(v1, v2);
        }

        static SimdType reciprocal(const SimdType& v)
        {
            SimdType estimate = _mm_rcp_ps(v);
            const SimdType two = _mm_set1_ps(2.0f);
            estimate = _mm_mul_ps(estimate, _mm_sub_ps(two, _mm_mul_ps(v, estimate)));
            return estimate;
        }
        
        static SimdType sqrt(const SimdType& v)
        {
            return _mm_sqrt_ps(v);
        }
        
        static SimdType min(const SimdType& v1, const SimdType& v2)
        {
            return _mm_min_ps( v1, v2 );
        }

        static SimdType max(const SimdType& v1, const SimdType& v2)
        {
            return _mm_max_ps( v1, v2 );
        }

        static SimdType cmpgt(const SimdType& v1, const SimdType& v2)
        {
            return _mm_cmpgt_ps( v1, v2 );
        }

        static SimdType cmpge(const SimdType& v1, const SimdType& v2)
        {
            return _mm_cmpge_ps( v1, v2 );
        }

        static SimdType cmplt(const SimdType& v1, const SimdType& v2)
        {
            return _mm_cmplt_ps( v1, v2 );
        }

        static SimdType cmple(const SimdType& v1, const SimdType& v2)
        {
            return _mm_cmple_ps( v1, v2 );
        }
        
        static SimdType complement(const SimdType& v)
        {
            return _mm_xor_ps( v, _mm_set1_ps(toFloat(0xffffffff)) );
        }
        
        static SimdType maskxor(const SimdType& v, const SimdType& mask)
        {
            return _mm_xor_ps( v, mask );
        }

        static SimdType maskor(const SimdType& v, const SimdType& mask)
        {
            return _mm_or_ps( v, mask );
        }

        static SimdType maskornot(const SimdType& v, const SimdType& mask)
        {
            return _mm_or_ps( complement(mask), v );
        }

        static SimdType maskand(const SimdType& v, const SimdType& mask)
        {
            return _mm_and_ps( v, mask );
        }
        
        static SimdType maskandnot(const SimdType& v, const SimdType& mask)
        {
            return _mm_andnot_ps( mask, v );
        }

        static SimdType select(const SimdType& v1, const SimdType& v2, const SimdType& mask)
        {
            // https://markplusplus.wordpress.com/2007/03/14/fast-sse-select-operation/
            return _mm_xor_ps( v2, _mm_and_ps( mask, _mm_xor_ps( v2, v1 ) ) );
//            return _mm_or_ps( _mm_and_ps( v1, mask ), _mm_andnot_ps( mask, v2 ) );
        }
        
        static float sum(const SimdType& v)
        {
            auto partial = _mm_hadd_ps(v, v);
            partial = _mm_hadd_ps(partial, partial);
            return partial[0];
        }
    };
}

