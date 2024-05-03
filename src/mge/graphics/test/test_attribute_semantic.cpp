// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/attribute_semantic.hpp"
#include "test/googletest.hpp"
#include "test/test_format.hpp"

TEST(attribute_semantic, output)
{
    test_format(mge::attribute_semantic::ANY, "ANY");
}