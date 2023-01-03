// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once

/**
 * @file mge/config.hpp
 * @brief Platform definitions.
 */

#ifdef _MSC_VER
#    define MGE_COMPILER_MSVC 1
#endif

#if defined(__WIN32__) || defined(_WIN32)
#    define MGE_OS_WINDOWS 1
#endif

#ifdef __APPLE__
#    define MGE_OS_MACOSX
#endif

namespace mge {

#if defined MGE_OS_WINDOWS
    namespace win32 {}
    namespace platform = win32;
#else
#    error Missing port.
#endif

} // namespace mge

#if defined(MGE_COMPILER_MSVC)
#    define MGE_FUNCTION_SIGNATURE __FUNCSIG__
#else
#    error Missing port
#endif

#define MGE_NO_RETURN [[noreturn]]
#if defined(MGE_COMPILER_MSVC)
#    define MGE_NO_INLINE __declspec(noinline)
#else
#    error Missing port
#endif

/**
 * Stringifies the arguments.
 */
#define MGE_STRINGIFY(...) #__VA_ARGS__
