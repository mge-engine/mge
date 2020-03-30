// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
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

#if defined(CDT_INDEXER)
#    define MGE_CORE_EXPORT __declspec(dllexport)
#else
/**
 * @def MGE_CORE_EXPORT
 * @brief Macro to declare class or function to be exported by/imported from
 * core library.
 */
#    ifdef BUILD_MGE_CORE
#        define MGE_CORE_EXPORT MGE_DLLEXPORT
#    else
#        define MGE_CORE_EXPORT MGE_DLLIMPORT
#    endif
#endif
