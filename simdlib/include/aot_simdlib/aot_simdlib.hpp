//
//  simdlib.h
//  VectorialHost
//
//  Created by Daniel Doubleday on 16/10/15.
//  Copyright © 2015 Daniel Doubleday. All rights reserved.
//

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               aot_simdlib
  vendor:           anyoddthing
  version:          0.0.1
  name:             Anyoddthing simdlib
  description:      Header only SIMD abstraction for SSE and NEON
  website:          http://www.anyoddthing.com
  license:          GPLv3

  dependencies:
  OSXFrameworks:
  iOSFrameworks:
  linuxLibs:
  linuxPackages:
  mingwLibs:

 END_JUCE_MODULE_DECLARATION
 */

#pragma once

#ifndef VECTORIAL_FORCED
    #if defined(__SSE__) || (_M_IX86_FP > 0) || (_M_X64 > 0)
        #define SIMDLIB_SSE
    #elif defined(__ARM_NEON__) 
        #define SIMDLIB_NEON
    #else
        #define SIMDLIB_SCALAR
    #endif
#endif

#include "SimdCommon.hpp"

#ifdef SIMDLIB_SSE
    #include "SimdSSE.hpp"
#elif defined(SIMDLIB_NEON)
    #include "SimdNeon.hpp"
#else
//    #include <array>
//    typedef std::array<float, 4> simd4f;
#endif

#include "SimdVec.hpp"
#include "Numerics.hpp"
#include "Memory.hpp"


