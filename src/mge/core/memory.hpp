// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
/** @file memory.hpp */
#pragma once
#include "mge/core/dllexport.hpp"
#include <memory>

/**
 * @def MGE_DECLARE_REF
 * @brief Declares reference type (shared pointer).
 * @param TYPE type used in the reference type, the type is assumed
 * to be a class (no struct, enum class or union).
 */
#define MGE_DECLARE_REF(TYPE)                                                  \
    class TYPE;                                                                \
    using TYPE##_ref = std::shared_ptr<TYPE>

/**
 * @def MGE_DECLARE_WEAK_REF
 * @brief Declares weak reference type (weak pointer).
 * @param TYPE type used in the reference type, the type is assumed
 * to be a class (no struct, enum class or union).
 */
#define MGE_DECLARE_WEAK_REF(TYPE)                                             \
    class TYPE;                                                                \
    using TYPE##_weak_ref = std::weak_ptr<TYPE>

namespace mge {
    /**
     * @brief Allocate memory.
     *
     * An @ref mge::out_of_memory exception is thrown in case memory
     * cannot be allocated.
     *
     * @param size number of bytes to allocate
     * @return pointer to allocated memory
     */
    MGECORE_EXPORT void* malloc(size_t size);

    /**
     * @brief Frees allocated memory.
     *
     * Passing a @c nullptr to the function is safe, no operation is
     * performed in this case.
     *
     * @param ptr pointer to memory to free.
     */
    MGECORE_EXPORT void free(void* ptr);

    /**
     * @brief Reallocate memory.
     *
     * This function is similar to @c realloc from the C standard
     * library. If the pointer is @c nullptr, the function behaves
     * like @ref mge::malloc. If the size is 0, the function behaves like
     * @ref mge::free.
     *
     * @param ptr pointer to memory to reallocate
     * @param size new size of memory block
     * @return pointer to reallocated memory
     */
    MGECORE_EXPORT void* realloc(void* ptr, size_t size);

    // support functions for memory_resource implementation
    MGECORE_EXPORT void* allocate(size_t bytes, size_t alignment);
    MGECORE_EXPORT void  deallocate(void* p, size_t bytes, size_t alignment);
} // namespace mge
