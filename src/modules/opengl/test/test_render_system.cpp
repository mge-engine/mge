// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "opengl_test.hpp"

namespace opengl {
    class test_render_system : public opengltest
    {};

    TEST_F(test_render_system, has_monitors)
    {
        auto monitors = render_system->monitors();
        EXPECT_GT(monitors.size(), 0u);
    }

    TEST_F(test_render_system, primary_monitor)
    {
        auto monitors = render_system->monitors();
        ASSERT_GT(monitors.size(), 0);
        auto primary_monitor = render_system->primary_monitor();
        EXPECT_TRUE(primary_monitor.get() != nullptr);
    }

} // namespace opengl
