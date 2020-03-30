// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/exception.hpp"
#include "test/googletest.hpp"

#define EXPECT_THROW_WITH_MESSAGE(expr, exception_class, exception_message)    \
    do {                                                                       \
        try {                                                                  \
            expr;                                                              \
            FAIL() << "Expected " << #exception_class << " to be thrown";      \
        } catch (const exception_class &ex) {                                  \
            EXPECT_STREQ(exception_message, ex.what());                        \
        }                                                                      \
    } while (false)
