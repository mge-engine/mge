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

    /**
     * @brief Allocate memory.
     *
     * @param sz number of bytes to allocate
     * @return pointer allocated memory, NULL if not possible to allocate
     */
    MGE_CORE_EXPORT void *malloc(size_t sz);
    /**
     * @brief Free memory.
     *
     * @param ptr pointer to free
     */
    MGE_CORE_EXPORT void free(void *ptr);
    /**
     * @brief Reallocate memory buffer.
     *
     * @param ptr   pointer to previously allocated memory
     * @param newsz new size
     * @return pointer to reallocated memory, may be same or different from
     *   passed pointer
     */
    MGE_CORE_EXPORT void *realloc(void *ptr, size_t newsz);

    /**
     * @brief Allocate memory and initialize.
     *
     * @param num   number of elements
     * @param size  size of one element
     * @return pointer to allocated and cleared memory
     */
    MGE_CORE_EXPORT void *calloc(size_t num, size_t size);

}
