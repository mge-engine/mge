/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "test/googletest.hpp"
#include "mge/core/exception.hpp"

class fex : public boost::exception
{};

TEST(exception, construct)
{
    mge::exception ex1;
    mge::exception ex2(ex1);
    fex foreign;
    mge::exception ex3(foreign);
}
