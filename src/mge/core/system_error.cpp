// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/system_error.hpp"

namespace mge {
    void
    system_error::clear()
    {
#ifdef MGE_OS_WINDOWS
        SetLastError(0);
#else
#  error Not implemented.
#endif
    }

#define DEFINE_EXCEPTION(clazz)                            \
    clazz::clazz()                                         \
    {}                                                     \
                                                           \
    clazz::clazz(const clazz& c)                           \
        :mge::exception(c)                                 \
    {}                                                     \
                                                           \
    clazz::clazz(clazz&& c)                                \
        :mge::exception(std::move(c))                      \
    {}                                                     \
                                                           \
    clazz&                                                 \
    clazz::operator = (const clazz& c)                     \
    {                                                      \
        mge::exception::operator =(c);                     \
        return *this;                                      \
    }                                                      \
                                                           \
    clazz::~clazz()                                        \
    {}

    DEFINE_EXCEPTION(system_error)
#undef DEFINE_EXCEPTION
}
