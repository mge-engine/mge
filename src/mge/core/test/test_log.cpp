// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/core/log.hpp"
#include "test/googletest.hpp"

TEST(log, wrap_in_closure_macro)
{
    auto c = MGE_WRAP_IN_CLOSURE(17);
    EXPECT_EQ(17, c());
}

TEST(log, wrap_list_in_closures)
{
    auto l = std::make_tuple(MGE_CLOSURE_LIST(1, 2));
}

// TEST(log, simple_log) { MGE_LOG("My {} is full of {}", "hoovercraft",
// "eels"); }