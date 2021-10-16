// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

/**
 * @def MGESCRIPT_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * input library.
 */
#ifdef BUILD_MGESCRIPT
#    define MGESCRIPT_EXPORT MGE_DLLEXPORT
#else
#    define MGESCRIPT_EXPORT MGE_DLLIMPORT
#endif
