// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/config.hpp"
#include <boost/core/noncopyable.hpp>

#include <memory>

#define MGE_DECLARE_REF(clazz)             \
class clazz;                               \
typedef std::shared_ptr<clazz> clazz##_ref

namespace mge {
    using noncopyable = boost::noncopyable;
}
