// mge - Modern Game Engine
// Copyright (c) 2019 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/dllexport.hpp"
#include <boost/preprocessor.hpp>

namespace mge {

    class MGECORE_EXPORT log
    {
    public:
        log();
        ~log();

        template <typename T> void call(T &&logged) {}
    };

#define MGE_WRAP_IN_CLOSURE(X) [&]() { return X; }

#define MGE_CLOSURE_LIST_1(_, __, EL) MGE_WRAP_IN_CLOSURE(EL),

#define MGE_CLOSURE_LIST(...)                                                  \
    BOOST_PP_SEQ_FOR_EACH(MGE_CLOSURE_LIST_1, _,                               \
                          BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))               \
    nullptr

#define MGE_LOG(...) mge::log().call(MGE_CLOSURE_LIST(__VA_ARGS__))
} // namespace mge