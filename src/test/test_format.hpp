#pragma once
#include "test/googletest.hpp"
#include <format>
#include <string>
namespace mge {

    template <typename E> inline void test_format(E e, const char* expected)
    {
        std::string s = std::format("{}", e);
        EXPECT_STREQ(s.c_str(), expected);
    }
} // namespace mge
