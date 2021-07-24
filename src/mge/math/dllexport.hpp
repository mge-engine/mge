// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/dllexport.hpp"

#ifdef BUILD_MGEMATH
#    define MGEMATH_EXPORT MGE_DLLEXPORT
#else
#    define MGEMATH_EXPORT MGE_DLLIMPORT
#endif
