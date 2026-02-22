// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/application/application.hpp"
#include "mge/application/loop.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/win32/window.hpp"
#include "test/applicationtest.hpp"
#include "test/googletest.hpp"
#include "mge/graphics/test/mock_render_system.hpp"

class windowtest_application : public mge::test_application
{
public:
    void setup()
    {
        m_window = std::make_shared<mge::win32::window>(
            m_mock_render_system,
            mge::extent(640, 480),
            mge::window_options::standard_options());
        m_window->show();
    }
    MOCK_render_system m_mock_render_system;
    mge::window_ref m_window;
};

TEST(win32_window, basic)
{
    EXPECT_TRUE(true);
}

MGE_REGISTER_IMPLEMENTATION(windowtest_application,
                            mge::application,
                            windowtest);
