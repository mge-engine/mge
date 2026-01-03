// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/shader_handle.hpp"
#include "mock_render_context.hpp"
#include "mock_shader.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(shader, shader_handle_lifecycle)
{
    auto ctx = std::make_shared<MOCK_render_context>();
    EXPECT_CALL(*ctx, on_create_shader(mge::shader_type::VERTEX))
        .Times(1)
        .WillOnce(Return(new MOCK_shader(*ctx, mge::shader_type::VERTEX)));
    auto sh = ctx->create_shader(mge::shader_type::VERTEX);
    EXPECT_TRUE(sh);
}