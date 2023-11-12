// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_type.hpp"
#include "test/googletest.hpp"
#include "test/test_stream_op.hpp"

TEST(shader_type, format)
{
    std::vector<std::pair<mge::shader_type, std::string>> test_cases = {
        {mge::shader_type::VERTEX, "VERTEX"},
        {mge::shader_type::FRAGMENT, "FRAGMENT"},
        {mge::shader_type::GEOMETRY, "GEOMETRY"},
        {mge::shader_type::TESSELATION_CONTROL, "TESSELATION_CONTROL"},
        {mge::shader_type::TESSELATION_EVALUATION, "TESSELATION_EVALUATION"},
        {mge::shader_type::COMPUTE, "COMPUTE"}
    };

    for (const auto& test_case : test_cases) {
        std::ostringstream oss;
        oss << test_case.first;
        EXPECT_EQ(test_case.second, oss.str());
    }
}

TEST(shader_type, formatter)
{
    std::vector<std::pair<mge::shader_type, std::string>> test_cases = {
        {mge::shader_type::VERTEX, "VERTEX"},
        {mge::shader_type::FRAGMENT, "FRAGMENT"},
        {mge::shader_type::GEOMETRY, "GEOMETRY"},
        {mge::shader_type::TESSELATION_CONTROL, "TESSELATION_CONTROL"},
        {mge::shader_type::TESSELATION_EVALUATION, "TESSELATION_EVALUATION"},
        {mge::shader_type::COMPUTE, "COMPUTE"}
    };

    for (const auto& test_case : test_cases) {
        auto str = std::format("{}", test_case.first);
        std::ostringstream oss;
        oss << str;
        EXPECT_EQ(test_case.second, oss.str());
    }
}