// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "dx12_test.hpp"

namespace mge::dx12 {

    void dx12test_application::setup()
    {
        m_render_system = mge::render_system::create("dx12");
        m_window = m_render_system->create_window(
            mge::extent(800, 600),
            mge::window_options::standard_options());
    }

    void dx12test::SetUp()
    {
        dx12test_application* app =
            dynamic_cast<dx12test_application*>(mge::application::instance());
        m_render_system = app->m_render_system;
        m_window = app->m_window;
    }

    void dx12test::TearDown()
    {
        m_window.reset();
        m_render_system.reset();
    }

    MGE_REGISTER_IMPLEMENTATION(dx12test_application,
                                mge::application,
                                dx12test);
} // namespace mge::dx12
APPLICATIONTEST_MAIN(dx12test);