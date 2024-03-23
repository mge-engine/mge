// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/configuration.hpp"
#include "mge/core/exception.hpp"
#include "mge/core/parameter.hpp"
#include "test/googletest.hpp"
#include <filesystem>

using namespace std::string_view_literals;

MGE_DEFINE_PARAMETER(std::string, test, test_parameter2, "a test parameter");

TEST(configuration, find_parameter)
{
    auto& p = mge::configuration::find_parameter("test"sv, "test_parameter2"sv);
    EXPECT_EQ("test/test_parameter2"sv, p.path().string());
}

TEST(configuration, find_parameter_throws_on_unknown)
{
    EXPECT_THROW(
        mge::configuration::find_parameter("test"sv, "unknown_parameter"sv),
        mge::exception);
}

TEST(configuration, store)
{
    std::filesystem::path cfgfile("test_core.json");
    std::filesystem::path cfgfile2("test_core.xml");
    if (std::filesystem::exists(cfgfile)) {
        std::filesystem::remove(cfgfile);
    }
    if (std::filesystem::exists(cfgfile2)) {
        std::filesystem::remove(cfgfile2);
    }

    MGE_PARAMETER(test, test_parameter2)
        .from_string("my hoovercraft is full of eels");
    mge::configuration::store();
    EXPECT_TRUE(std::filesystem::exists(cfgfile));
}