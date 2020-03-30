// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx11_test.hpp"
#include "mge/core/configuration.hpp"
#include "mge/graphics/window.hpp"

namespace dx11 {
    mge::render_system_ref dx11test::render_system;

    void dx11test::SetUpTestCase()
    {
        mge::configuration config;
        render_system = mge::render_system::create("dx11");
        if (!render_system) {
            FAIL() << "Cannot create dx11 render system";
        }
        config.set("debug", "1");
        render_system->configure(config);
    }

    void            dx11test::TearDownTestCase() { render_system.reset(); }
    mge::window_ref dx11test_with_window::window;

    void dx11test_with_window::SetUpTestCase()
    {
        dx11test::SetUpTestCase();
        window = render_system->create_window(mge::rectangle(0, 0, 1600, 900),
                                              mge::window_options());
        window->assign_thread();
    }

    void dx11test_with_window::TearDownTestCase()
    {
        window.reset();
        dx11test::TearDownTestCase();
    }
} // namespace dx11
