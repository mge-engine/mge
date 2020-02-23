// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/shader/program.hpp"

using namespace mge;
using namespace mge::shader;

TEST(program, set_source)
{
    program p(program_type::VERTEX);
    std::string src("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    p.set_source(src);
}
