// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"
#include "test/googlemock.hpp"

class MOCK_render_system : public mge::render_system
{
    class mock_capabilities : public mge::render_system::capabilities
    {
    public:
        mock_capabilities() = default;
    };

public:
    MOCK_render_system()
    {
        m_capabilities = std::unique_ptr<capabilities>(new mock_capabilities());
    }
    ~MOCK_render_system() {}

    MOCK_METHOD(mge::window_ref,
                create_window,
                (const mge::extent&, const mge::window_options&),
                (override));
    MOCK_METHOD(std::span<mge::monitor_ref>, monitors, (), (override));
};