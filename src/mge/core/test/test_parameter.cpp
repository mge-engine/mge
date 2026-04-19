// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/core/parameter.hpp"
#include "test/googletest.hpp"
using namespace std::string_view_literals;
MGE_DEFINE_PARAMETER(bool, test, test_parameter1, "a test parameter");
MGE_DEFINE_PARAMETER(std::vector<int>,
                     test,
                     test_parameter_vec,
                     "a test parameter");
MGE_DEFINE_PARAMETER_WITH_DEFAULT(
    int, test, test_parameter_default, "param with default", 42);

TEST(parameter, simple_access)
{
    EXPECT_EQ(mge::path("test/test_parameter1"),
              MGE_PARAMETER(test, test_parameter1).path());
    EXPECT_EQ("a test parameter"sv,
              MGE_PARAMETER(test, test_parameter1).description());
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
}

TEST(parameter, set_value)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
    MGE_PARAMETER(test, test_parameter1).set_value(true);
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).has_value());
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).get());
    MGE_PARAMETER(test, test_parameter1).reset();
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
}

TEST(parameter, set_value_wrong_type)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
    try {
        MGE_PARAMETER(test, test_parameter1).set_value(std::string("hurz"));
        FAIL() << "set_value should throw an exception";
    } catch (const mge::exception& ex) {
        EXPECT_TRUE(std::string(ex.what()).find("Invalid type for parameter") !=
                    std::string::npos);
    }
}

#if 0
TEST(parameter, from_string_to_string)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter1).has_value());
    MGE_PARAMETER(test, test_parameter1).from_string("1");
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).has_value());
    EXPECT_EQ(std::string("1"),
              MGE_PARAMETER(test, test_parameter1).to_string());
    MGE_PARAMETER(test, test_parameter1).reset();
}
#endif

TEST(parameter, vec)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter_vec).has_value());
    EXPECT_TRUE((MGE_PARAMETER(test, test_parameter_vec).value().empty()));

    // MGE_PARAMETER(test, test_parameter_vec).from_string("1, 2, 3");
    // EXPECT_TRUE(MGE_PARAMETER(test, test_parameter_vec).has_value());
    // EXPECT_EQ(std::string("1, 2, 3"),
    //           MGE_PARAMETER(test, test_parameter_vec).to_string());
    // MGE_PARAMETER(test, test_parameter_vec).reset();
}

TEST(parameter, change_handler)
{
    bool changed = false;
    MGE_PARAMETER(test, test_parameter1).set_change_handler([&changed]() {
        changed = true;
    });
    MGE_PARAMETER(test, test_parameter1).notify_change();
    EXPECT_TRUE(changed);
    MGE_PARAMETER(test, test_parameter1).set_change_handler(nullptr);
}

TEST(parameter, default_value)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter_default).has_value());
    EXPECT_EQ(42, MGE_PARAMETER(test, test_parameter_default).get());
}

TEST(parameter, default_value_override)
{
    MGE_PARAMETER(test, test_parameter_default).value() = 99;
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter_default).has_value());
    EXPECT_EQ(99, MGE_PARAMETER(test, test_parameter_default).get());
    MGE_PARAMETER(test, test_parameter_default).reset();
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter_default).has_value());
    EXPECT_EQ(42, MGE_PARAMETER(test, test_parameter_default).get());
}

TEST(parameter, get_with_fallback)
{
    EXPECT_FALSE(MGE_PARAMETER(test, test_parameter_default).has_value());
    EXPECT_EQ(100, MGE_PARAMETER(test, test_parameter_default).get(100));
    MGE_PARAMETER(test, test_parameter_default).reset();
}

TEST(parameter, json_read_write)
{
    mge::json::json j;
    j["test"]["test_parameter1"] = true;
    MGE_PARAMETER(test, test_parameter1)
        .read_value(j["test"]["test_parameter1"]);
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).has_value());
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).get());

    mge::json::json out;
    MGE_PARAMETER(test, test_parameter1).write_value(out);
    EXPECT_TRUE(out.get<bool>());

    MGE_PARAMETER(test, test_parameter1).reset();
}

TEST(parameter, as_typed)
{
    MGE_PARAMETER(test, test_parameter1).set_value(true);
    EXPECT_TRUE(MGE_PARAMETER(test, test_parameter1).as<bool>());
    MGE_PARAMETER(test, test_parameter1).reset();
}
