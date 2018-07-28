// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/stdexceptions.hpp"

namespace mge {
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

    DEFINE_EXCEPTION(illegal_state)
    DEFINE_EXCEPTION(illegal_argument)
    DEFINE_EXCEPTION(out_of_range)
#undef DEFINE_EXCEPTION
}
