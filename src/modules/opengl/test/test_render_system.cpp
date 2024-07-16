// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"
#include <string_view>

class render_system_test : public mge::opengl::opengltest
{};

TEST_F(render_system_test, monitors)
{
    auto monitors = m_render_system->monitors();
    EXPECT_GE(monitors.size(), 1u);
}

TEST_F(render_system_test, capabilities)
{
    using namespace std::literals;

    const auto& caps = m_render_system->system_capabilities();
    ASSERT_GE(caps.shader_languages().size(), 1);
    const auto& sl1 = *(caps.shader_languages().begin());
    EXPECT_EQ("glsl"sv, sl1.name());
}
