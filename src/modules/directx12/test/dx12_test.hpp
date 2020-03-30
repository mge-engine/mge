// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once

#include "mge/graphics/render_system.hpp"
#include "mge/graphics/window.hpp"
#include "test/googlemock.hpp"

namespace dx12 {
    class dx12test : public ::testing::Test
    {
    public:
        static void                   SetUpTestCase();
        static void                   TearDownTestCase();
        static mge::render_system_ref render_system;
    };

    class dx12test_with_window : public dx12test
    {
    public:
        static void            SetUpTestCase();
        static void            TearDownTestCase();
        static mge::window_ref window;
    };
} // namespace dx12
