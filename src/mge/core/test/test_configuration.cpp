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
MGE_DEFINE_PARAMETER(std::vector<int>,
                     test,
                     test_parameter_vec_save,
                     "a test vector parameter");

TEST(configuration, find_parameter)
{
    auto& p = mge::configuration::find_parameter("test"sv, "test_parameter2"sv);
    EXPECT_EQ("test/test_parameter2"sv, p.path().generic_string());
}

TEST(configuration, find_parameter_throws_on_unknown)
{
    EXPECT_THROW(
        mge::configuration::find_parameter("test"sv, "unknown_parameter"sv),
        mge::exception);
}
#if 0
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

    MGE_PARAMETER(test, test_parameter_vec_save).reset();
    MGE_PARAMETER(test, test_parameter_vec_save).add_value("1");
    MGE_PARAMETER(test, test_parameter_vec_save).add_value("2");
    MGE_PARAMETER(test, test_parameter_vec_save).add_value("3");
    mge::configuration::store();
    EXPECT_TRUE(std::filesystem::exists(cfgfile));
    MGE_PARAMETER(test, test_parameter_vec_save).reset();
    mge::configuration::load();
    EXPECT_EQ(3, MGE_PARAMETER(test, test_parameter_vec_save).values().size());
}
#endif