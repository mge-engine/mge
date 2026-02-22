// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/dllexport.hpp"

/**
 * @def MGEUI_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * ui library.
 */
#ifdef BUILD_MGEUI
#    define MGEUI_EXPORT MGE_DLLEXPORT
#else
#    define MGEUI_EXPORT MGE_DLLIMPORT
#endif
