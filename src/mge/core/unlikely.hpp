// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/config.hpp"

#ifdef MGE_COMPILER_GNUC

#define MGE_LIKELY(x)           __builtin_expect(!!(x), 1)
#define MGE_UNLIKELY(x)         __builtin_expect(!!(x), 0)
#define MGE_UNPREDICTABLE(x)    (x)

#elif MGE_COMPILER_CLANG

#define MGE_LIKELY(x)           __builtin_expect(!!(x), 1)
#define MGE_UNLIKELY(x)         __builtin_expect(!!(x), 0)
#define MGE_UNPREDICTABLE(x)    __builtin_unpredictable(!!(x))

#elif MGE_COMPILER_MSVC

#define MGE_LIKELY(X)           (X)
#define MGE_UNLIKELY(X)         (X)
#define MGE_UNPREDICTABLE(X)    (X)

#else

#define MGE_LIKELY(X)           (X)
#define MGE_UNLIKELY(X)         (X)
#define MGE_UNPREDICTABLE(X)    (X)

#endif


