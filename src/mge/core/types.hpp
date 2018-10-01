// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/config.hpp"
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
#define MGE_DECLARE_REF(clazz)             \
class clazz;                               \
typedef std::shared_ptr<clazz> clazz##_ref

namespace mge {
    /**
     * @brief Type that cannot be copied or assigned.
     */
    typedef boost::noncopyable noncopyable;
}

/**
 * Stringifies the argument.
 * @param X argument to stringify
 */
#define MGE_STRINGIFY(X) #X
