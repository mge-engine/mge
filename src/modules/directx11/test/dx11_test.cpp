// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"

namespace mge::dx11 {

    void dx11test_application::setup()
    {
        m_render_system = mge::render_system::create("dx11");
        m_window = m_render_system->create_window(
            mge::extent(800, 600),
            mge::window_options::standard_options());
    }

    void dx11test::SetUp()
    {
        dx11test_application* app =
            dynamic_cast<dx11test_application*>(mge::application::instance());
        m_render_system = app->m_render_system;
        m_window = app->m_window;
    }

    void dx11test::TearDown()
    {
        m_window.reset();
        m_render_system.reset();
    }

    MGE_REGISTER_IMPLEMENTATION(dx11test_application,
                                mge::application,
                                dx11test);
} // namespace mge::dx11
APPLICATIONTEST_MAIN(dx11test);