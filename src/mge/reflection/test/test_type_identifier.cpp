// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/reflection/type_identifier.hpp"
#include "test/googletest.hpp"

namespace mge::reflection {

    TEST(type_identifier, basic)
    {
        type_identifier id(typeid(int), false, false);
        EXPECT_EQ(id.type_index(), typeid(int));
        EXPECT_FALSE(id.is_const());
        EXPECT_FALSE(id.is_volatile());
    }
} // namespace mge::reflection
