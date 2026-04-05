// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

#ifdef BUILD_MGEREFLECTION
#    define MGEREFLECTION_EXPORT MGE_DLLEXPORT
#else
#    define MGEREFLECTION_EXPORT MGE_DLLIMPORT
#endif
