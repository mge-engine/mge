// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/config.hpp"
#include "mge/core/new.hpp"
#include <boost/core/noncopyable.hpp>

/**
 * @file mge/core/types.hpp
 * @brief Common definitions.
 */

#include <memory>

/**
 * @brief Declare a reference type for a class.
 *
 * The macro declares a @c clazz_ref type
 * which is a @c std::shared_ptr of @c clazz.
 * @param clazz class to declare
 */
#define MGE_DECLARE_REF(clazz)                                                 \
    class clazz;                                                               \
    using clazz##_ref = std::shared_ptr<clazz>

namespace mge {
    /**
     * @brief Type that cannot be copied or assigned.
     */
    using noncopyable = boost::noncopyable;
} // namespace mge

/**
 * Stringifies the argument.
 * @param X argument to stringify
 */
#define MGE_STRINGIFY(...) #__VA_ARGS__

/**
 * @brief Marks a variable as unused.
 * @param X variable
 */
#define MGE_UNUSED(X) X
