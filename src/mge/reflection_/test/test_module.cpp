// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/reflection/module.hpp"

TEST(module, global_module)
{
    auto& m = mge::reflection::module::global_module();
    EXPECT_EQ(std::string(""), m.name());
}

TEST(module, std_module)
{
    auto& m = mge::reflection::module::get<std::string>();
    EXPECT_EQ(std::string("std"), m.name());
}

