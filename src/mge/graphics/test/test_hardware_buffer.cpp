// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mock_hardware_buffer.hpp"
#include "mock_render_context.hpp"
#include "mock_render_system.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(hardware_buffer, construct)
{
    auto rs = std::make_shared<MOCK_render_system>();
    auto ctx = std::make_shared<MOCK_render_context>(*rs);

    MOCK_hardware_buffer buffer(*ctx, mge::buffer_type::INDEX, 1024);
    EXPECT_EQ(buffer.type(), mge::buffer_type::INDEX);
    EXPECT_EQ(buffer.size(), 1024);
}