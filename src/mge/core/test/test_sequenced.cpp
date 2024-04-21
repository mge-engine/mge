// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/sequenced.hpp"
#include "test/googletest.hpp"

namespace mge {

    TEST(sequenced, sequenced)
    {
        sequenced<int> a;
        sequenced<int> b;
        EXPECT_LT(a, b);
        EXPECT_LE(a, b);
        EXPECT_GT(b, a);
        EXPECT_GE(b, a);
        EXPECT_NE(a, b);
    }

    TEST(sequenced, sequenced_move)
    {
        sequenced<int> a;
        sequenced<int> b(std::move(a));
        EXPECT_EQ(0, a.sequence());
        EXPECT_FALSE(a.has_value());
        EXPECT_NE(0, b.sequence());
    }

    TEST(sequenced, sequenced_move_assign)
    {
        sequenced<int> a;
        sequenced<int> b;
        b = std::move(a);
        EXPECT_EQ(0, a.sequence());
        EXPECT_FALSE(a.has_value());
        EXPECT_NE(0, b.sequence());
    }

    TEST(sequenced, sequenced_copy_assign)
    {
        sequenced<int>                a;
        sequenced<int>                b;
        sequenced<int>::sequence_type b_seq = b.sequence();
        b = a;
        EXPECT_EQ(a.sequence(), b.sequence());
        EXPECT_NE(b_seq, b.sequence());
    }
} // namespace mge