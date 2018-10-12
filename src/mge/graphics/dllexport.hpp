// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"

#ifdef CDT_INDEXER
#  define MGE_GRAPHICS_EXPORT __declspec(dllexport)
#else
#  ifdef BUILD_MGE_GRAPHICS
#    define MGE_GRAPHICS_EXPORT MGE_DLLEXPORT
#  else
#    define MGE_GRAPHICS_EXPORT MGE_DLLIMPORT
#  endif
#endif
