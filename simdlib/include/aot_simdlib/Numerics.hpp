//
//  Numerics.hpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 01/11/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#pragma once

#include "Numerics.inc"

namespace simd
{
    template<class T>
    constexpr T pi = T(3.141592653589793238462643383279502884197169399375105820974944592307816406286);
    constexpr size_t stride = 4;

    constexpr size_t pad(size_t i)
    {
        return (i / 4) * 4  + (i % 4 + 3) / 4 * 4;
    }
    
    template <size_t DefaultSize>
    struct recipes
    {
        template<typename CallbackT, size_t Size = DefaultSize>
        static inline void loop(CallbackT&& callback)
        {
            LOOP([&](size_t i)
            {
                callback(i);
            });
        }

        SIMD_RECIPE memset(float* dest, const float value)
        {
            LOOP([&](size_t i)
            {
                Vec v(value);
                v.store(dest + i);
            });
        }

        SIMD_RECIPE memcopy(float* dest, const float* src)
        {
            LOOP([&](size_t i)
            {
                Vec v(src + i);
                v.store(dest + i);
            });
        }

        SIMD_RECIPE add(float* dest, const float* src1, const float* src2)
        {
            LOOP([&](size_t i)
            {
                Vec v1(src1 + i), v2(src2 + i);
                Vec v = v1 + v2;
                v.store(dest + i);
            });
        }

        SIMD_RECIPE mpy(float* dest, const float* src1, const float* src2)
        {
            LOOP([&](size_t i)
            {
                Vec v1(src1 + i), v2(src2 + i);
                Vec v = v1 * v2;
                v.store(dest + i);
            });
        }

        SIMD_RECIPE mpy(float* dest, const float* src, const float fac)
        {
            LOOP([&](size_t i)
            {
                Vec v1(src + i);
                Vec v = v1 * fac;
                v.store(dest + i);
            });
        }

        SIMD_RECIPE ramp(float* dest, const float* src, const float fact, const float step)
        {
            for (size_t i = 0; i < Size; ++i)
            {
                dest[i] = src[i] * (fact + i * step);
            }
        }

        SIMD_RECIPE initPhase(float *dest, Vec init, const float phaseDelta)
        {
            Vec wrapped = init - 2.0f;
            Vec mask = wrapped >= -1.0f;
            init = mask.select(wrapped, init);
            init.store(dest);
            if (Size > simd::stride)
            {
                LOOP_(simd::stride, Size)([&](size_t i)
                {
                    init += phaseDelta * simd::stride;
                    wrapped = init - 2.0f;
                    mask = wrapped >= -1.0f;
                    init = mask.select(wrapped, init);
                    init.store(dest + i);
                });
            }
        }

        SIMD_RECIPE initPhase(float *dest, const float phaseDelta)
        {
            Vec init(0, phaseDelta, 2 * phaseDelta, 3 * phaseDelta);
            initPhase<Size>(dest, init, phaseDelta);
        }

        SIMD_RECIPE initPhase(float *dest, const float phaseDelta, const float startPhase)
        {
            Vec init(0, phaseDelta, 2 * phaseDelta, 3 * phaseDelta);
            initPhase<Size>(dest, init + startPhase, phaseDelta);
        }

        /**
        * Updates the phase with a constant delta.
        *
        * \param dest alligned dest address with values in range [-1, 1]
        * \param phaseDelta phase delta which needs to be within [0, 0.25]
        */
        SIMD_RECIPE updatePhase(float *dest, const float phaseDelta)
        {
            Vec v(dest + Size - simd::stride);
            v += phaseDelta * simd::stride;
            Vec wrapped = v - 2.0f;
            Vec mask = wrapped >= -1.0f;
            v = mask.select(wrapped, v);
            v.store(dest);
            
            if (Size > simd::stride)
            {
                LOOP_(simd::stride, Size)([&](size_t i)
                {
                    v += phaseDelta * simd::stride;
                    wrapped = v - 2.0f;
                    mask = wrapped >= -1.0f;
                    v = mask.select(wrapped, v);
                    v.store(dest + i);
                });
            }
        }

        /**
        * Wraps the phase so values [-1, 2] are transformed to [-1, 1].
        *
        * \param dest alligned dest address with values in range [-1, 2]
        */
        SIMD_RECIPE wrapPhase(float *dest)
        {
            LOOP([&](size_t i)
            {
                Vec v(dest + i);
                Vec wrapped = v - 2.0f;
                Vec mask = wrapped >= -1.0f;
                v = mask.select(wrapped, v);
                v.store(dest + i);
            });
        }

        template<size_t Size = DefaultSize, bool UseAmp = false>
        static inline void fastSin(float *dest, const float *phase, const float *amp = nullptr)
        {
            for (size_t i = 0; i < Size; i += simd::stride)
            {
                // Factorial constants so code is easier to read.

                Vec currPhase(phase + i);
                Vec rFoldBack = Vec(1) - currPhase;
                Vec lFoldBack = Vec(-1) - currPhase;
                Vec rMask = currPhase > 0.5f;
                Vec lMask = currPhase < -0.5f;

                Vec wrappedPhase = lMask.select(lFoldBack, rMask.select(rFoldBack, currPhase));

                Vec x = wrappedPhase * simd::pi<float>;
                Vec x2 = (x * x);

                float IF3  = (1.0) / (2 * 3);
                float IF5  = IF3 / (4 * 5);
                float IF7  = -IF5 / (6 * 7);
                float IF9  = -IF7 / (8 * 9);
                float IF11 = IF9 / (10 * 11);

                /* Taylor expansion out to x**11/11! factored into multiply-adds */
                Vec v1 = x2 * IF11 + IF9;
                Vec v2 = v1 * x2 + IF7;
                Vec v3 = v2 * x2 + IF5;
                Vec v4 = v3 * x2 - IF3;
                Vec v5 = v4 * x2 + 1;

                Vec result = x * v5;

                if (UseAmp)
                {
                    result *= Vec(amp + i);
                }

                result.store(dest + i);
            }
        }
    };
    
    namespace math
    {
        static inline float sum(const float* src, size_t n)
        {
            Vec partial(src);
            for (size_t i = simd::stride; i < n; i += simd::stride)
            {
                partial += Vec(src + i);
            }
            
            return partial.sum();
        }
    }
}











