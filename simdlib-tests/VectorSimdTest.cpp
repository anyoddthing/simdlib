//
//  ScalarSimdTest.cpp
//  VectorialHost
//
//  Created by Daniel Doubleday on 25/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

#include <stdio.h>
#include "catch.hpp"
#include "TestHelper.hpp"

using OpsTest = TestHelper<simd_t>;

TEST_CASE("Vector implementation of simd ops")
{
    #include "OpsTest.inc"
}
