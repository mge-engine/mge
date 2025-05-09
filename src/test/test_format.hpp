// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/core/format.hpp"
#include "test/googletest.hpp"
#include <string>

namespace mge {

    template <typename E> inline void test_format(E e, const char* expected)
    {
        std::string s = fmt::format("{}", e);
        EXPECT_STREQ(s.c_str(), expected);
    }
} // namespace mge
