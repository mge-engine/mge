// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/exception.hpp"
#include "mge/core/type_name.hpp"
#include "test/googlemock.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(exception, rethrow)
{
    EXPECT_DEATH(mge::rethrow(), "");
}

TEST(exception, construct_simple)
{
    mge::exception ex;
}

TEST(exception, set_info)
{
    mge::exception ex;
    ex.set_info(mge::exception::source_file(__FILE__))
        .set_info(mge::exception::source_line(__LINE__))
        .set_info(mge::exception::function(MGE_FUNCTION_SIGNATURE))
        .set_info(mge::exception::type_name(mge::type_name<decltype(ex)>()));
    std::stringstream msg;
    msg << ex.details();
    std::string message = msg.str();
    EXPECT_TRUE(message.find("Exception type: mge::exception") !=
                std::string::npos);
    EXPECT_TRUE(message.find("test_exception.cpp") != std::string::npos);
    EXPECT_TRUE(message.find("Exception location: ") != std::string::npos);
    EXPECT_TRUE(message.find("exception_set_info_Test::TestBody") !=
                std::string::npos);
}

TEST(exception, set_info_with_location)
{
    mge::exception    ex(std::source_location::current());
    std::stringstream msg;
    msg << ex.details();
    std::string message = msg.str();
    EXPECT_TRUE(message.find("test_exception.cpp") != std::string::npos);
    EXPECT_TRUE(message.find("Exception location: ") != std::string::npos);
    EXPECT_TRUE(
        message.find("exception_set_info_with_location_Test::TestBody") !=
        std::string::npos);
}

TEST(exception, set_info_using_operator)
{
    mge::exception ex;
    ex << MGE_CALLED_FUNCTION(FOOBAR) << "test message";
    std::stringstream msg;
    msg << ex.details();
    std::string message = msg.str();

    EXPECT_TRUE(message.find("Calling library/system function: FOOBAR") !=
                std::string::npos);
    EXPECT_TRUE(message.find("Exception message: test message") !=
                std::string::npos);
    EXPECT_TRUE(message.find("test_exception.cpp") != std::string::npos);
    EXPECT_TRUE(message.find("Exception location: ") != std::string::npos);
    EXPECT_TRUE(
        message.find("exception_set_info_using_operator_Test::TestBody") !=
        std::string::npos);
}

TEST(exception, throw_macro)
{
    try {
        MGE_THROW(mge::exception) << std::string("wurst");
    } catch (const mge::exception& ex) {
        EXPECT_STREQ("wurst", ex.what());
    }
}

TEST(exception, throw_macro_with_cause)
{
    try {
        try {
            MGE_THROW(mge::exception) << "wurst";
        } catch (const mge::exception& ex) {
            MGE_THROW_WITH_CAUSE(mge::exception, ex) << "cheese";
        }
    } catch (const mge::exception& ex2) {
        EXPECT_STREQ("cheese", ex2.what());
        auto cause = ex2.get<mge::exception::cause>();
        EXPECT_TRUE(cause.has_value());
        EXPECT_STREQ("wurst", cause->what());
    }
}
