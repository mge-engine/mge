// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "vulkan_test.hpp"
#include "mge/core/configuration.hpp"
#include "mge/graphics/window.hpp"

namespace vulkan
{
    mge::render_system_ref vulkantest::render_system;

    void vulkantest::SetUpTestCase()
    {
        mge::configuration config;
        render_system = mge::render_system::create("vulkan");
        if (!render_system)
        {
            FAIL() << "Cannot create vulkan render system";
        }
        config.set("debug", "1");
        render_system->configure(config);
    }

    void vulkantest::TearDownTestCase()
    {
        render_system.reset();
    }
    mge::window_ref vulkantest_with_window::window;

    void vulkantest_with_window::SetUpTestCase()
    {
        vulkantest::SetUpTestCase();
        window = render_system->create_window(mge::rectangle(0, 0, 1600, 900),
            mge::window_options());
        window->assign_thread();
    }

    void vulkantest_with_window::TearDownTestCase()
    {
        window.reset();
        vulkantest::TearDownTestCase();
    }
}
