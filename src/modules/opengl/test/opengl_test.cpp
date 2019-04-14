/*
 * Copyright (c) 2017 by Alexander Schroeder
 */
#include "opengl_test.hpp"
#include "mge/core/configuration.hpp"
#include "mge/graphics/window.hpp"

namespace opengl
{
mge::render_system_ref opengltest::render_system;

void opengltest::SetUpTestCase()
{
    mge::configuration config;
    render_system = mge::render_system::create("opengl");
    if (!render_system)
    {
        FAIL() << "Cannot create opengl render system";
    }
    config.set("debug", "1");
    render_system->configure(config);
}

void opengltest::TearDownTestCase()
{
    render_system.reset();
}
mge::window_ref opengltest_with_window::window;

void opengltest_with_window::SetUpTestCase()
{
    opengltest::SetUpTestCase();
    window = render_system->create_window(mge::rectangle(0, 0, 1600, 900),
                                          mge::window_options());
    window->assign_thread();
}

void opengltest_with_window::TearDownTestCase()
{
    window.reset();
    opengltest::TearDownTestCase();
}
}
