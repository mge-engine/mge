// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <cstdlib>

/**
 * @file mge/core/memory.hpp
 * @brief Memory management.
 */

namespace mge {

    MGE_CORE_EXPORT void *malloc(size_t sz);
    MGE_CORE_EXPORT void free(void *ptr);
    MGE_CORE_EXPORT void *realloc(void *ptr, size_t newsz);
    MGE_CORE_EXPORT void *calloc(size_t num, size_t size);

}
