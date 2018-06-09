// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/config.hpp"
#include <memory>

#define MGE_DECLARE_REF(clazz)             \
class clazz;                               \
typedef std::shared_ptr<clazz> clazz##_ref

namespace mge {
    class noncopyable
    {
    public:
        noncopyable() = default;
        ~noncopyable() = default;
        noncopyable(const noncopyable&) = delete;
        noncopyable(noncopyable&&) = delete;
        noncopyable& operator =(const noncopyable&) = delete;
    };
}
