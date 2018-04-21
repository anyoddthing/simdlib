//
//  FIRBench.cpp
//  simdlib
//
//  Created by Daniel Doubleday on 19.04.18.
//  Copyright © 2018 Daniel Doubleday. All rights reserved.
//

#include "bench.hpp"

//#define FILTER_TAP_NUM 20
static float filter_taps[160] = {
  0.0019390782882137887,
  0.0032384617106349716,
  -0.0005598729703174639,
  -0.006658539329411668,
  -0.00441065171178432,
  0.005836241148952741,
  0.006321788895877844,
  -0.008621820501626892,
  -0.013638193706077547,
  0.00735534817882479,
  0.02125296166861005,
  -0.005352091275918336,
  -0.03351422027330926,
  -0.0021817362402696363,
  0.05008216981174255,
  0.0177217186563712,
  -0.0799491618225474,
  -0.06008679798019613,
  0.17134096520779674,
  0.4237090613389178,
  0.4237090613389178,
  0.17134096520779674,
  -0.06008679798019613,
  -0.0799491618225474,
  0.0177217186563712,
  0.05008216981174255,
  -0.0021817362402696363,
  -0.03351422027330926,
  -0.005352091275918336,
  0.02125296166861005,
  0.00735534817882479,
  -0.013638193706077547,
  -0.008621820501626892,
  0.006321788895877844,
  0.005836241148952741,
  -0.00441065171178432,
  -0.006658539329411668,
  -0.0005598729703174639,
  0.0032384617106349716,
  0.0019390782882137887,
  0.0019390782882137887,
  0.0032384617106349716,
  -0.0005598729703174639,
  -0.006658539329411668,
  -0.00441065171178432,
  0.005836241148952741,
  0.006321788895877844,
  -0.008621820501626892,
  -0.013638193706077547,
  0.00735534817882479,
  0.02125296166861005,
  -0.005352091275918336,
  -0.03351422027330926,
  -0.0021817362402696363,
  0.05008216981174255,
  0.0177217186563712,
  -0.0799491618225474,
  -0.06008679798019613,
  0.17134096520779674,
  0.4237090613389178,
  0.4237090613389178,
  0.17134096520779674,
  -0.06008679798019613,
  -0.0799491618225474,
  0.0177217186563712,
  0.05008216981174255,
  -0.0021817362402696363,
  -0.03351422027330926,
  -0.005352091275918336,
  0.02125296166861005,
  0.00735534817882479,
  -0.013638193706077547,
  -0.008621820501626892,
  0.006321788895877844,
  0.005836241148952741,
  -0.00441065171178432,
  -0.006658539329411668,
  -0.0005598729703174639,
  0.0032384617106349716,
  0.0019390782882137887,
  0.0019390782882137887,
  0.0032384617106349716,
  -0.0005598729703174639,
  -0.006658539329411668,
  -0.00441065171178432,
  0.005836241148952741,
  0.006321788895877844,
  -0.008621820501626892,
  -0.013638193706077547,
  0.00735534817882479,
  0.02125296166861005,
  -0.005352091275918336,
  -0.03351422027330926,
  -0.0021817362402696363,
  0.05008216981174255,
  0.0177217186563712,
  -0.0799491618225474,
  -0.06008679798019613,
  0.17134096520779674,
  0.4237090613389178,
  0.4237090613389178,
  0.17134096520779674,
  -0.06008679798019613,
  -0.0799491618225474,
  0.0177217186563712,
  0.05008216981174255,
  -0.0021817362402696363,
  -0.03351422027330926,
  -0.005352091275918336,
  0.02125296166861005,
  0.00735534817882479,
  -0.013638193706077547,
  -0.008621820501626892,
  0.006321788895877844,
  0.005836241148952741,
  -0.00441065171178432,
  -0.006658539329411668,
  -0.0005598729703174639,
  0.0032384617106349716,
  0.0019390782882137887,
  0.0019390782882137887,
  0.0032384617106349716,
  -0.0005598729703174639,
  -0.006658539329411668,
  -0.00441065171178432,
  0.005836241148952741,
  0.006321788895877844,
  -0.008621820501626892,
  -0.013638193706077547,
  0.00735534817882479,
  0.02125296166861005,
  -0.005352091275918336,
  -0.03351422027330926,
  -0.0021817362402696363,
  0.05008216981174255,
  0.0177217186563712,
  -0.0799491618225474,
  -0.06008679798019613,
  0.17134096520779674,
  0.4237090613389178,
  0.4237090613389178,
  0.17134096520779674,
  -0.06008679798019613,
  -0.0799491618225474,
  0.0177217186563712,
  0.05008216981174255,
  -0.0021817362402696363,
  -0.03351422027330926,
  -0.005352091275918336,
  0.02125296166861005,
  0.00735534817882479,
  -0.013638193706077547,
  -0.008621820501626892,
  0.006321788895877844,
  0.005836241148952741,
  -0.00441065171178432,
  -0.006658539329411668,
  -0.0005598729703174639,
  0.0032384617106349716,
  0.0019390782882137887
};

template <uint TapSize>
struct FIRBench
{
    FIRBench() : _coefs(TapSize), _scoefs(simd::stride * TapSize), _lscoefs(TapSize * TapSize), _buffer(TapSize)
    {
        reset();
        setCoefs(filter_taps);
        initSourceBuffer();
    }

    void initSourceBuffer()
    {
        // just generate some noise to filter
        uint bufferSize = 1024;
        _sourceBuffer = simd::AlignedMemory(bufferSize);
        std::generate(_sourceBuffer.begin(), _sourceBuffer.end(), [] ()
        {
            return std::rand() * 2.0f / RAND_MAX - 1;
        });
    }
    
    void setCoefs(float* coefs)
    {
        std::copy(coefs, coefs + TapSize, _coefs.begin());
        setSimdCoefs(coefs);
        setSimdCoefs2(coefs);
    }

    template <typename ContainerT>
    void setSimdCoefs(const ContainerT& coefs)
    {
        for (auto i = 0; i < TapSize; ++i)
        {
            for (auto j = 0; j < simd::stride; ++j)
            {
                _scoefs[i * simd::stride + j] = coefs[(i + j) % TapSize];
            }
        }
    }
    
    template <typename ContainerT>
    void setSimdCoefs2(const ContainerT& coefs)
    {
        auto offset = 0;
        
        for (auto i = 0; i < TapSize; ++i)
        {
            for (auto j = 0; j < TapSize; ++j)
            {
                _lscoefs[offset * TapSize + j] = coefs[(i + j) % TapSize];
            }
            
            offset = (offset == 0 ? TapSize - 1 : offset - 1);
        }
    }
    
    void reset()
    {
        _index = 0;
        _coefs.zero_mem();
        _buffer.zero_mem();
    }

    NOINLINE baseLineTest(float* optKiller)
    {
        _index = 0;
        float result = 0;
        for (float val : _sourceBuffer)
        {
            result = sisdFilter(val);
        }
        *optKiller = result;
    }
    
    NOINLINE simdTest(float* optKiller)
    {
        _index = 0;
        float result = 0;
        for (float val : _sourceBuffer)
        {
            result = simdFilter(val);
        }
        *optKiller = result;
    }
    
    NOINLINE simdTest2(float* optKiller)
    {
        _index = 0;
        float result = 0;
        for (float val : _sourceBuffer)
        {
            result = simdFilter2(val);
        }
        *optKiller = result;
    }
    
    float sisdFilter(float sample)
    {
        _buffer[_index] = sample;
        
        float out = 0;
        size_t k;
        for (k = 0; k < TapSize - _index; ++k)
            out += _buffer[(_index + k)] * _coefs[k];

        for (size_t j = 0; j < _index; ++j)
            out += _buffer[j] * _coefs[j + k];

        _index = (_index == 0 ? TapSize - 1 : _index - 1);

        return out;
    }

    float simdFilter(float sample)
    {
        using namespace simd;
        
        _buffer[_index] = sample;
        Vec partial(0.0f);
        for (auto i = 0; i < TapSize; i += stride)
        {
            auto coefIdx = (TapSize - _index + i) % TapSize;
            
            Vec coefs(_scoefs.data() + coefIdx * simd::stride);
            Vec samples(_buffer.data() + i);
            
            partial += coefs * samples;
        }
        
        _index = (_index == 0 ? TapSize - 1 : _index - 1);
        return partial.sum();
    }

    float simdFilter2(float sample)
    {
        using namespace simd;
        
        _buffer[_index] = sample;
        Vec partial(0.0f);
        for (auto i = 0; i < TapSize; i += stride)
        {
            Vec coefs(_lscoefs.data() + _index * TapSize + i);
            Vec samples(_buffer.data() + i);
            
            partial += coefs * samples;
        }
        
        _index = (_index == 0 ? TapSize - 1 : _index - 1);
        return partial.sum();
    }
    

private:
    uint _index;
    simd::AlignedMemory _coefs;
    simd::AlignedMemory _scoefs;
    simd::AlignedMemory _lscoefs;
    simd::AlignedMemory _buffer;
    simd::AlignedMemory _sourceBuffer;
};

static FIRBench<20> filter20;
static FIRBench<40> filter40;
static FIRBench<80> filter80;
static FIRBench<160> filter160;

static float optkiller;

BENCHMARK("FIR BL20", [](benchpress::context* ctx)
{
    BENCH_ITER filter20.baseLineTest(&optkiller);
})

BENCHMARK("FIR SIMD20", [](benchpress::context* ctx)
{
    BENCH_ITER filter20.simdTest(&optkiller);
})

BENCHMARK("FIR LSIMD20", [](benchpress::context* ctx)
{
    BENCH_ITER filter20.simdTest2(&optkiller);
})

BENCHMARK("FIR BL40", [](benchpress::context* ctx)
{
    BENCH_ITER filter40.baseLineTest(&optkiller);
})

BENCHMARK("FIR SIMD40", [](benchpress::context* ctx)
{
    BENCH_ITER filter40.simdTest(&optkiller);
})

BENCHMARK("FIR LSIMD40", [](benchpress::context* ctx)
{
    BENCH_ITER filter40.simdTest2(&optkiller);
})

BENCHMARK("FIR BL80", [](benchpress::context* ctx)
{
    BENCH_ITER filter80.baseLineTest(&optkiller);
})

BENCHMARK("FIR SIMD80", [](benchpress::context* ctx)
{
    BENCH_ITER filter80.simdTest(&optkiller);
})

BENCHMARK("FIR LSIMD80", [](benchpress::context* ctx)
{
    BENCH_ITER filter80.simdTest2(&optkiller);
})

BENCHMARK("FIR BL160", [](benchpress::context* ctx)
{
    BENCH_ITER filter160.baseLineTest(&optkiller);
})

BENCHMARK("FIR SIMD160", [](benchpress::context* ctx)
{
    BENCH_ITER filter160.simdTest(&optkiller);
})

BENCHMARK("FIR LSIMD160", [](benchpress::context* ctx)
{
    BENCH_ITER filter160.simdTest2(&optkiller);
})





