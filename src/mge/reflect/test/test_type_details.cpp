// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "mge/reflect/type_details.hpp"
#include "test/googletest.hpp"
#include <string_view>

TEST(type_details, pod)
{
    using namespace std::literals;
    EXPECT_EQ("char"sv, mge::reflect::type_details<char>().name());
    EXPECT_EQ("unsigned char"sv,
              mge::reflect::type_details<unsigned char>().name());
    EXPECT_EQ("int8_t"sv, mge::reflect::type_details<int8_t>().name());
}
