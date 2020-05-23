// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"
#include "test/googletest.hpp"

TEST(exception, construct)
{
    mge::exception ex;
    mge::exception ex2(ex);
    mge::exception ex3(std::move(ex2));
    mge::exception ex4;

    ex4 = ex3;
}

TEST(exception, extract_source_file)
{
    try {
        MGE_THROW(mge::exception);
        FAIL() << "No exception thrown";
    } catch (const mge::exception &ex) {
        auto file_info = ex.get<mge::exception::source_file>();
        ASSERT_TRUE(file_info != std::nullopt);
        EXPECT_STRNE("", file_info.value());
    }
}

TEST(exception, thrown_exception_has_stacktrace)
{
    try {
        MGE_THROW(mge::exception);
        FAIL() << "Expect exception to be thrown";
    } catch (const mge::exception &ex_catched) {
        const mge::stacktrace st =
            ex_catched.get<mge::exception::stack>().value();
        std::cout << st;
        EXPECT_TRUE(st.size() > 0);
    }
}

TEST(exception, exception_with_message)
{
    try {
        MGE_THROW(mge::exception) << "foo"
                                  << "bar";
    } catch (const mge::exception &ex_catched) {
        EXPECT_STREQ("foobar", ex_catched.what());
    }
}
