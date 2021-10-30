// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
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
     * An :any:`mge::out_of_memory` exception is thrown in case memory
     * cannot be allocated.
     *
     * @param size number of bytes to allocate
     * @return pointer to allocated memory
     */
    MGECORE_EXPORT void* malloc(size_t size);

    /**
     * @brief Frees allocated memory.
     *
     * Passing a :any:`nullptr` to the function is safe, no operation is
     * performed in this case.
     *
     * @param ptr pointer to memory to free.
     */
    MGECORE_EXPORT void free(void* ptr);
} // namespace mge
