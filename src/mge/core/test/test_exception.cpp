/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "test/googletest.hpp"
#include "mge/core/exception.hpp"


TEST(exception, construct)
{
    mge::exception ex;
    mge::exception ex2(ex);
    mge::exception ex3(std::move(ex2));
    mge::exception ex4;

    ex4 = ex3;
}


TEST(exception, thrown_exception_has_stacktrace)
{
    try {
        MGE_THROW(mge::exception());
        FAIL() << "Expect exception to be thrown";
    } catch(const mge::exception& ex_catched) {
        //std::cout << boost::diagnostic_information(ex_catched, true);
        const mge::stacktrace *st = ex_catched.info<mge::excinfo_stack>();
        EXPECT_TRUE(st != nullptr);
    }
}

TEST(exception, exception_with_message)
{
    try {
        MGE_THROW(mge::exception() << mge::excinfo_desc("foobar"));
    } catch(const mge::exception& ex_catched) {
        EXPECT_STREQ("foobar", ex_catched.what());
    }
}
