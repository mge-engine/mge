// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/core/dllexport.hpp"

#ifdef CDT_INDEXER
#  define MGE_SCRIPT_EXPORT __declspec(dllexport)
#else
#  ifdef BUILD_MGE_SCRIPT
#    define MGE_SCRIPT_EXPORT MGE_DLLEXPORT
#  else
#    define MGE_SCRIPT_EXPORT MGE_DLLIMPORT
#  endif
#endif
