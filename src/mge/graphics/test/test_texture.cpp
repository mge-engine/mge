// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/memory_image.hpp"
#include "mge/graphics/texture_type.hpp"
#include "mock_render_context.hpp"
#include "mock_render_system.hpp"
#include "mock_texture.hpp"
#include "test/googletest.hpp"

using namespace testing;

TEST(texture, set_data)
{
    auto              rs = std::make_shared<MOCK_render_system>();
    auto              ctx = std::make_shared<MOCK_render_context>(*rs);
    MOCK_texture      tex(*ctx, mge::texture_type::TYPE_2D);
    mge::extent       ext(1024, 1024);
    mge::memory_image img(
        mge::image_format(mge::image_format::data_format::RGBA,
                          mge::data_type::UINT8),
        ext);
    EXPECT_CALL(tex,
                set_data(mge::image_format(mge::image_format::data_format::RGBA,
                                           mge::data_type::UINT8),
                         ext,
                         img.data(),
                         img.binary_size()))
        .Times(1);
    tex.set_data(img);
}