// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "dx12_test.hpp"
#include "mge/core/configuration.hpp"
#include "mge/graphics/window.hpp"

namespace dx12 {
    mge::render_system_ref dx12test::render_system;

    void dx12test::SetUpTestCase()
    {
        mge::configuration config;
        render_system = mge::render_system::create("dx12");
        if (!render_system) {
            FAIL() << "Cannot create dx12 render system";
        }
        config.set("debug", "1");
        render_system->configure(config);
    }

    void            dx12test::TearDownTestCase() { render_system.reset(); }
    mge::window_ref dx12test_with_window::window;

    void dx12test_with_window::SetUpTestCase()
    {
        dx12test::SetUpTestCase();
        window = render_system->create_window(mge::rectangle(0, 0, 1600, 900),
                                              mge::window_options());
        window->assign_thread();
    }

    void dx12test_with_window::TearDownTestCase()
    {
        window.reset();
        dx12test::TearDownTestCase();
    }
} // namespace dx12
