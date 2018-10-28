// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "test/googletest.hpp"
#include <sstream>

namespace mge {
    template <typename E, int MAX>
    void test_stream_operator()
    {
        for(int i=0; i<=MAX; ++i) {
            E f = (E)i;
            std::stringstream ss;
            ss << f;
            std::string f_str = ss.str();
            EXPECT_FALSE(f_str.empty());
            EXPECT_TRUE( strncmp(f_str.c_str(), "INVALID(", 8) != 0)
                << "Found output " << f_str << " for value " << i;
        }

        E f = (E)(MAX + 1);
        std::stringstream cmp;
        cmp << "INVALID(" << (MAX+1) << ")";
        std::stringstream ss;
        ss << f;
        EXPECT_EQ(cmp.str(), ss.str());
    }

    template <typename E>
    void test_stream_output(E e, const char *expected)
    {
        std::stringstream ss;
        ss << e;
        std::string ss_str(ss.str());
        EXPECT_STREQ(ss_str.c_str(), expected);
    }
}
