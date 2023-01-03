// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/dllexport.hpp"

/**
 * @def MGEAPPLICATION_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * application library.
 */
#ifdef BUILD_MGEAPPLICATION
#    define MGEAPPLICATION_EXPORT MGE_DLLEXPORT
#else
#    define MGEAPPLICATION_EXPORT MGE_DLLIMPORT
#endif
