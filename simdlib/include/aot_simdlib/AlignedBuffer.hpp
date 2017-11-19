//
//  AlignedBuffer.hpp
//  simdlib
//
//  Created by Daniel Doubleday on 07/11/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#ifndef AlignedBuffer_h
#define AlignedBuffer_h

#include <array>

static_assert(alignof(std::max_align_t) >= 16, "Can't guarantee simd alignment");

namespace simd
{
    template <size_t Size>
    struct AlignedBuffer
    {
        alignas(16) std::array<float, Size> data;
        
        float& operator[]       (const size_t index)       { return data[index]; }
        const float& operator[] (const size_t index) const { return data[index]; }
        
        constexpr size_t size() const { return Size; }
        
        float* ptr(size_t start = 0)
        {
            return data.data() + start;
        }
        
        void set(size_t start, float v1, float v2, float v3, float v4)
        {
            data[start + 0] = v1;
            data[start + 1] = v2;
            data[start + 2] = v3;
            data[start + 3] = v4;
        }
    };
  
}

#endif /* AlignedBuffer_h */
