// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "test/googletest.hpp"
#include "mge/shader/program.hpp"

using namespace mge;
using namespace mge::shader;

struct test_element
{
    program *p;
    int i;

    test_element(program *pp, int ii)
    {
        p = pp;
        i = ii;
    }
};

TEST(program, simple_create)
{
    program p(program_type::VERTEX);
    auto pi = p.create<int>(42);
    EXPECT_EQ(42, *pi);
}

TEST(program, element_create)
{
    program p(program_type::VERTEX);
    auto te = p.create<test_element>(42);
    EXPECT_EQ(&p, te->p);
    EXPECT_EQ(42, te->i);
}
