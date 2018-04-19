//
//  simdlib-bench.hpp
//  simdlib
//
//  Created by Daniel Doubleday on 17.04.18.
//  Copyright © 2018 Daniel Doubleday. All rights reserved.
//

#pragma once

#include "benchpress/benchpress.hpp"

#include <chrono>
#include <iostream>
#include <array>

#include "aot_simdlib.hpp"

#define BENCH_ITER for (size_t i = 0; i < ctx->num_iterations(); ++i)
#define NOINLINE __attribute__ ((noinline)) void


