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

using ScalarType = std::array<float, 4>;
using OpsTest = TestHelper<ScalarType>;

TEST_CASE("Scalar implementation of simd ops")
{

    #include "OpsTest.inc"

}
