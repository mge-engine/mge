// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/data_type.hpp"
#include "mge/graphics/render_context.hpp"
#include "mge/graphics/texture.hpp"
#include "opengl_test.hpp"
#include "test/benchmark.hpp"


class texture_test : public mge::opengl::opengltest
{};

TEST_F(texture_test, create_texture)
{
    auto& context = m_window->render_context();
    auto  texture = context.create_texture(mge::texture_type::TYPE_2D);
}

TEST_F(texture_test, bench_texture_creation)
{
    mge::benchmark().show_results().run("texture_creation", [&]() {
        auto& context = m_window->render_context();
        auto  texture = context.create_texture(mge::texture_type::TYPE_2D);
    });
}
