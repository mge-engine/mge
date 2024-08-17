// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/memory_resource.hpp"
#include "test/googletest.hpp"

TEST(memory_source, allocate_shared)
{
    auto stringptr =
        std::allocate_shared<std::string>(mge::memory_resource::allocator,
                                          "Hello, World!");
}