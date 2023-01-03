// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

/**
 * @def MGEINPUT_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * input library.
 */
#ifdef BUILD_MGEINPUT
#    define MGEINPUT_EXPORT MGE_DLLEXPORT
#else
#    define MGEINPUT_EXPORT MGE_DLLIMPORT
#endif
