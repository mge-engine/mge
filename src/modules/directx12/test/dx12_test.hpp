// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/application/application.hpp"
#include "mge/application/loop.hpp"
#include "mge/graphics/graphics_fwd.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/graphics/window.hpp"
#include "mge/win32/window.hpp"
#include "test/applicationtest.hpp"
#include "test/googlemock.hpp"
#include "test/googletest.hpp"

namespace mge::dx12 {
    class dx12test_application : public mge::test_application
    {
    public:
        void setup();

        mge::render_system_ref m_render_system;
        mge::window_ref        m_window;
    };

    class dx12test : public ::testing::Test
    {
    public:
        dx12test() = default;
        ~dx12test() = default;

        void SetUp() override;
        void TearDown() override;

        mge::render_system_ref m_render_system;
        mge::window_ref        m_window;
    };

} // namespace mge::dx12
