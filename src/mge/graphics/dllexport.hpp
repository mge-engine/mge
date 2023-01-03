// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

/**
 * @def MGEGRAPHICS_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * graphics library.
 */
#ifdef BUILD_MGEGRAPHICS
#    define MGEGRAPHICS_EXPORT MGE_DLLEXPORT
#else
#    define MGEGRAPHICS_EXPORT MGE_DLLIMPORT
#endif
