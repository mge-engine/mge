// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"
#include "mge/reflection/signature.hpp"

TEST(signature, construct)
{
    mge::reflection::signature s;
    mge::reflection::signature s1({std::type_index(typeid(int)),
                                   std::type_index(typeid(std::string))});
}


TEST(signature, stream_output)
{
    mge::reflection::signature s({std::type_index(typeid(int)),
                                  std::type_index(typeid(std::string))});
    mge::test_stream_output(s, "(int, class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >)");
}
