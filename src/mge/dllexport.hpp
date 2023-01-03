// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

/**
 * @file   mge/core/dllexport.hpp
 * @brief  Macros to declare import/export for dynamic libraries.
 */

/**
 * @def MGE_DLLEXPORT
 * @brief Macro to declare class or function to be exported from DLL.
 */

/**
 * @def MGE_DLLIMPORT
 * @brief Macro to declare class or function to be imported from DLL.
 */

#if defined(_MSC_VER)
#    define MGE_DLLEXPORT __declspec(dllexport)
#    define MGE_DLLIMPORT __declspec(dllimport)
#else
#    define MGE_DLLEXPORT
#    define MGE_DLLIMPORT
#endif
