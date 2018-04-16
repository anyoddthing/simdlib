//
//  simdlib_neon.h
//  VectorialHost
//
//  Created by Daniel Doubleday on 24/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#pragma once

#include <arm_neon.h>

namespace simd
{
    using simd_t = float32x4_t;
    
    namespace detail
    {
        static simd_t reciprocal(const simd_t v)
        {
            simd_t estimate = vrecpeq_f32(v);
            estimate = vmulq_f32(vrecpsq_f32(estimate, v), estimate);
            estimate = vmulq_f32(vrecpsq_f32(estimate, v), estimate);
            return estimate;
        }
        
        static void rsqrt_1iteration(const simd_t& v, simd_t& estimate)
        {
            simd_t estimate2 = vmulq_f32(estimate, v);
            estimate = vmulq_f32(estimate, vrsqrtsq_f32(estimate2, estimate));
        }

        static simd_t rsqrt(simd_t v)
        {
            simd_t estimate = vrsqrteq_f32(v);
            rsqrt_1iteration(v, estimate);
            rsqrt_1iteration(v, estimate);
            return estimate;
        }

        static simd_t sqrt(simd_t v) {

            return vreinterpretq_f32_u32(vandq_u32( vtstq_u32(vreinterpretq_u32_f32(v),  
                                                              vreinterpretq_u32_f32(v)), 
                                                    vreinterpretq_u32_f32(
                                                      reciprocal(rsqrt(v)))
                                                  )
                                        );
        }
    }

    template <>
    struct SimdOps<float32x4_t>
    {
        using SimdType = float32x4_t;
        
        static uint getIval(const SimdType &simd, size_t index)
        {
            return toUInt(simd[index]);
        }
        
        static void setIval(SimdType &simd, size_t index, uint val)
        {
            simd[index] = toFloat(val);
        }        
        
        static SimdType create(float v1, float v2, float v3, float v4)
        {
            const float32_t d[4] = { v1, v2, v3, v4 };
            return vld1q_f32(d);
        }
        
        static SimdType create(float v)
        {
            return vdupq_n_f32(v);
        }
        
        static SimdType load(const float* src)
        {
            return vld1q_f32(src);
        }
        
        static void store(const SimdType &v, float* dest)
        {
            vst1q_f32(dest, v);
        }
        
        static SimdType add(const SimdType &v1, const SimdType &v2)
        {
            return vaddq_f32(v1, v2);
        }
        
        static SimdType sub(const SimdType &v1, const SimdType &v2)
        {
            return vsubq_f32(v1, v2);
        }
        
        static SimdType mul(const SimdType &v1, const SimdType &v2)
        {
            return vmulq_f32(v1, v2);
        }

        static SimdType div(const SimdType &v1, const SimdType &v2)
        {
            SimdType recip = detail::reciprocal(v2);
            return vmulq_f32(v1, recip);
        }

        static SimdType reciprocal(const SimdType &v)
        {
            return detail::reciprocal(v);
        }
        
        static SimdType sqrt(const SimdType &v)
        {
            return detail::sqrt(v);
        }
        
        static SimdType min(const SimdType &v1, const SimdType &v2)
        {
            return vminq_f32( v1, v2 );
        }

        static SimdType max(const SimdType &v1, const SimdType &v2)
        {
            return vmaxq_f32( v1, v2 );
        }

        static SimdType cmpgt(const SimdType &v1, const SimdType &v2)
        {
            return vcgtq_f32( v1, v2 );
        }

        static SimdType cmpge(const SimdType &v1, const SimdType &v2)
        {
            return vcgeq_f32( v1, v2 );
        }

        static SimdType cmplt(const SimdType &v1, const SimdType &v2)
        {
            return vcltq_f32( v1, v2 );
        }

        static SimdType cmple(const SimdType &v1, const SimdType &v2)
        {
            return vcleq_f32( v1, v2 );
        }
        
        static SimdType complement(const SimdType &v)
        {
            return vmvnq_u32( v );
        }

        static SimdType maskxor(const SimdType &v, const SimdType &mask)
        {
            return veorq_u32( v, mask );
        }

        static SimdType maskor(const SimdType &v, const SimdType &mask)
        {
            return vorrq_u32( v, mask );
        }

        static SimdType maskornot(const SimdType &v, const SimdType &mask)
        {
            return vornq_u32( v, mask );
        }

        static SimdType maskand(const SimdType &v, const SimdType &mask)
        {
            return vandq_u32( v, mask );
        }

        static SimdType maskandnot(const SimdType &v, const SimdType &mask)
        {
            return vbicq_u32( v, mask );
        }
        
        static SimdType select(const SimdType &v1, const SimdType &v2, const SimdType &mask)
        {
            return vorrq_u32( vandq_u32( v1, mask ), vbicq_u32( v2, mask ) );
        }
    };

}


