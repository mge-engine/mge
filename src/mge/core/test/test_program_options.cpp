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

TEST(program_options, positional_option)
{
    program_options po;
    po.positional("file1", "first file", program_options::value<std::string>())
        .positional("file2",
                    "second file",
                    program_options::value<std::string>());
}

TEST(program_options, parse_normal_option)
{
    program_options po;
    po.option("t,test", "test option");

    const char*              argv[] = {"test", "-t"};
    int                      argc = 2;
    program_options::options options;
    po.parse(argc, argv, options);

    EXPECT_TRUE(options.has_option("test"));
}

TEST(program_options, parse_unknown_option)
{
    program_options po;
    po.option("t,test", "test option");

    const char*              argv[] = {"test", "-x"};
    int                      argc = 2;
    program_options::options options;
    try {
        po.parse(argc, argv, options);
        FAIL() << "Expected exception not thrown";
    } catch (const mge::unknown_option& e) {
        EXPECT_STREQ("Unknown option: -x", e.what());
    }
}

TEST(program_options, parse_unknown_option_store_unrecognized)
{
    program_options po;
    po.option("t,test", "test option").stored_unrecognized();

    const char*              argv[] = {"test", "-x"};
    int                      argc = 2;
    program_options::options options;
    po.parse(argc, argv, options);
    EXPECT_TRUE(options.has_any_unrecognized());
    EXPECT_STREQ("-x", options.unrecognized()[0].c_str());
}

TEST(program_options, parse_option_long)
{
    program_options po;
    po.option("t,test", "test option");

    const char*              argv[] = {"test", "--test"};
    int                      argc = 2;
    program_options::options options;
    po.parse(argc, argv, options);

    EXPECT_TRUE(options.has_option("test"));
}

TEST(program_options, parse_option_store_value)
{
    program_options po;
    po.option("t,test", "test option", program_options::value<std::string>());

    const char*              argv[] = {"test", "--test", "abc"};
    int                      argc = 3;
    program_options::options options;
    po.parse(argc, argv, options);

    EXPECT_TRUE(options.has_option("test"));
    EXPECT_STREQ("abc",
                 std::any_cast<std::string>(options.option("test")).c_str());
}

TEST(program_options, parse_option_store_value_composing)
{
    program_options po;
    po.option("t,test",
              "test option",
              program_options::value<std::string>().composing());

    const char* argv[] = {"test", "--test", "abc", "--test", "def"};
    int         argc = 5;
    program_options::options options;
    po.parse(argc, argv, options);

    EXPECT_TRUE(options.has_option("test"));
    EXPECT_EQ(
        2,
        std::any_cast<std::vector<std::string>>(options.option("test")).size());
    EXPECT_STREQ(
        "abc",
        std::any_cast<std::vector<std::string>>(options.option("test"))[0]
            .c_str());
    EXPECT_STREQ(
        "def",
        std::any_cast<std::vector<std::string>>(options.option("test"))[1]
            .c_str());
}

TEST(program_options, parse_option_collect_positionals)
{
    program_options po;
    po.positional("file", "file name", program_options::value<std::string>())
        .positional("file2",
                    "second file",
                    program_options::value<std::string>());
    po.option("t,test", "test option", program_options::value<std::string>());

    const char* argv[] = {"test", "--test", "abc", "file1", "file2"};
    int         argc = 5;
    program_options::options options;
    po.parse(argc, argv, options);

    EXPECT_TRUE(options.has_option("test"));
    EXPECT_STREQ("abc",
                 std::any_cast<std::string>(options.option("test")).c_str());
    EXPECT_TRUE(options.has_positional("file"));
    EXPECT_STREQ(
        "file1",
        std::any_cast<std::string>(options.positional("file")).c_str());
    EXPECT_TRUE(options.has_positional("file2"));
    EXPECT_STREQ(
        "file2",
        std::any_cast<std::string>(options.positional("file2")).c_str());
}
