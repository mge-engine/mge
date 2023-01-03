// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "vulkan_test.hpp"

namespace mge::vulkan {

    void vulkantest_application::setup()
    {
        m_render_system = mge::render_system::create("vulkan");
        m_window = m_render_system->create_window(
            mge::extent(800, 600),
            mge::window_options::standard_options());
    }

    void vulkantest::SetUp()
    {
        vulkantest_application* app =
            dynamic_cast<vulkantest_application*>(mge::application::instance());
        m_render_system = app->m_render_system;
        m_window = app->m_window;
    }

    void vulkantest::TearDown()
    {
        m_window.reset();
        m_render_system.reset();
    }

    MGE_REGISTER_IMPLEMENTATION(vulkantest_application,
                                mge::application,
                                vulkantest);
} // namespace mge::vulkan
APPLICATIONTEST_MAIN(vulkantest);