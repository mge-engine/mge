// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/program_options.hpp"
#include "test/googletest.hpp"

using namespace mge;

TEST(program_options, empty)
{
    program_options po;
    EXPECT_EQ(0, po.size());
}

TEST(program_options, add_option)
{
    program_options po;
    po.option("test", "test option");
    EXPECT_EQ(1, po.size());
}

TEST(program_options, add_2_options)
{
    program_options po;
    po.option("t,test", "test option").option("v,verbose", "verbose option");
    EXPECT_EQ(2, po.size());
}

TEST(program_options, option_output)
{
    program_options po;
    po.option("t,test", "test option").option("v,verbose", "verbose option");
    std::stringstream ss;
    ss << po;
    EXPECT_STREQ("  -t [ --test ]     test option\n"
                 "  -v [ --verbose ]  verbose option\n",
                 ss.str().c_str());
}

TEST(program_options, option_with_arg)
{
    program_options po;
    po.option("test", "test option", program_options::value<int>());
    std::stringstream ss;
    ss << po;
    EXPECT_STREQ("  --test <arg>  test option\n", ss.str().c_str());
}
