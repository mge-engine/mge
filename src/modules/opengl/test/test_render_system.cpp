// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"

class render_system_test : public mge::opengl::opengltest
{};

TEST_F(render_system_test, monitors)
{
    auto monitors = m_render_system->monitors();
    EXPECT_GE(monitors.size(), 1u);
}
