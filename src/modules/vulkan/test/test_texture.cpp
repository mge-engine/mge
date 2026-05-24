// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/image_format.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/texture.hpp"
#include "mge/graphics/texture_usage.hpp"
#include "vulkan_test.hpp"

class texture_test : public mge::vulkan::vulkantest
{};

TEST_F(texture_test, create_texture)
{
    auto& context = m_window->render_context();
    auto  tex     = context.create_texture(mge::texture_type::TYPE_2D);
    EXPECT_TRUE(tex);
}

TEST_F(texture_test, create_render_target_texture)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::RGBA,
                          mge::data_type::UINT8);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    EXPECT_TRUE(tex);
}

TEST_F(texture_test, render_target_texture_has_render_target_usage)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::RGBA,
                          mge::data_type::UINT8);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    ASSERT_TRUE(tex);
    EXPECT_EQ(tex->usage(), mge::texture_usage::RENDER_TARGET);
}

TEST_F(texture_test, render_target_texture_type)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::RGBA,
                          mge::data_type::UINT8);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    ASSERT_TRUE(tex);
    EXPECT_EQ(tex->type(), mge::texture_type::TYPE_2D);
}

TEST_F(texture_test, create_depth_stencil_render_target_texture)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::DEPTH_STENCIL,
                          mge::data_type::UINT32);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    EXPECT_TRUE(tex);
}

TEST_F(texture_test, create_depth_float_render_target_texture)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::DEPTH,
                          mge::data_type::FLOAT);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    EXPECT_TRUE(tex);
}

TEST_F(texture_test, depth_texture_has_render_target_usage)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::DEPTH_STENCIL,
                          mge::data_type::UINT32);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    ASSERT_TRUE(tex);
    EXPECT_EQ(tex->usage(), mge::texture_usage::RENDER_TARGET);
}

TEST_F(texture_test, depth_texture_type)
{
    auto&             context = m_window->render_context();
    mge::extent       ext(256, 256);
    mge::image_format fmt(mge::image_format::data_format::DEPTH_STENCIL,
                          mge::data_type::UINT32);
    auto tex = context.create_render_target_texture(
        mge::texture_type::TYPE_2D, fmt, ext);
    ASSERT_TRUE(tex);
    EXPECT_EQ(tex->type(), mge::texture_type::TYPE_2D);
}
