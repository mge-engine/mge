// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

/**
 * @def MGECORE_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * core library.
 */
#ifdef BUILD_MGECORE
#    define MGECORE_EXPORT MGE_DLLEXPORT
#else
#    define MGECORE_EXPORT MGE_DLLIMPORT
#endif
