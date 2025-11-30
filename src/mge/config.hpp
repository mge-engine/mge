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

#if (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
#    define MGE_COMPILER_GCC 1
#endif

#if defined(__APPLE__) && defined(__clang__)
#    define MGE_COMPILER_APPLECLANG 1
#endif

#if defined(__WIN32__) || defined(_WIN32)
#    define MGE_OS_WINDOWS 1
#endif

#ifdef __APPLE__
#    define MGE_OS_MACOSX
#endif

#ifdef __linux__
#    define MGE_OS_LINUX 1
#endif

namespace mge {

#if defined MGE_OS_WINDOWS
    namespace win32 {}
    namespace platform = win32;
#elif defined MGE_OS_LINUX
    namespace linux {}
    namespace platform = linux;
#elif defined MGE_OS_MACOSX
    namespace macosx {}
    namespace platform = macosx;
#else
#    error Missing port.
#endif

} // namespace mge

#if defined(MGE_COMPILER_MSVC)
#    define MGE_FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(MGE_COMPILER_GCC)
#    define MGE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(MGE_COMPILER_APPLECLANG)
#    define MGE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#else
#    error Missing port
#endif

#define MGE_NO_RETURN [[noreturn]]
#if defined(MGE_COMPILER_MSVC)
#    define MGE_NO_INLINE __declspec(noinline)
#elif defined(MGE_COMPILER_GCC)
#    define MGE_NO_INLINE __attribute__((noinline))
#elif defined(MGE_COMPILER_APPLECLANG)
#    define MGE_NO_INLINE __attribute__((noinline))
#else
#    error Missing port
#endif

/**
 * Stringifies the arguments.
 */
#define MGE_STRINGIFY(...) #__VA_ARGS__
