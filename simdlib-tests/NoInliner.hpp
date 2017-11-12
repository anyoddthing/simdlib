//
//  NoInliner.hpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 25/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#ifndef NoInliner_hpp
#define NoInliner_hpp

#include <stdio.h>
#include "aot_simdlib.hpp"

struct NoInliner
{
    void doSomeMath(float *input, float *output);    
    void testDevision(float *input, float *output);
    void doSomeNumerics(float* output, const float *src, const float phaseIncrement);
    
    template <size_t Size>
    void testCrossFunctionCallOptimization(float* output, const float* src1, const float* src2, const float* src3);
};

#endif /* NoInliner_hpp */
