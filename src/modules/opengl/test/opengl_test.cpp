// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"

namespace mge::opengl {

    void opengltest_application::setup()
    {
        m_render_system = mge::render_system::create("opengl");
        m_window = m_render_system->create_window(
            mge::extent(800, 600),
            mge::window_options::standard_options());
    }

    void opengltest::SetUp()
    {
        opengltest_application* app =
            dynamic_cast<opengltest_application*>(mge::application::instance());
        m_render_system = app->m_render_system;
        m_window = app->m_window;
    }

    void opengltest::TearDown()
    {
        m_window.reset();
        m_render_system.reset();
    }

    MGE_REGISTER_IMPLEMENTATION(opengltest_application,
                                mge::application,
                                opengltest);
} // namespace mge::opengl
APPLICATIONTEST_MAIN(opengltest);