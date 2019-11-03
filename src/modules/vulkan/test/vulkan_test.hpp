// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "test/googlemock.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/window.hpp"

namespace vulkan
{
    class vulkantest : public ::testing::Test
    {
    public:
        static void SetUpTestCase();
        static void TearDownTestCase();
        static mge::render_system_ref render_system;
    };

    class vulkantest_with_window : public vulkantest
    {
    public:
        static void SetUpTestCase();
        static void TearDownTestCase();
        static mge::window_ref window;
    };
}
