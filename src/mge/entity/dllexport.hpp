// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

/**
 * @def MGEENTITY_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * core library.
 */
#ifdef BUILD_MGEENTITY
#    define MGEENTITY_EXPORT MGE_DLLEXPORT
#else
#    define MGEENTITY_EXPORT MGE_DLLIMPORT
#endif
