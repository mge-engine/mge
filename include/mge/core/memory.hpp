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
