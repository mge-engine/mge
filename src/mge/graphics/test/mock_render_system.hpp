// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"
#include "test/googlemock.hpp"

namespace mge {
    class mock_render_system : public render_system
    {
    public:
        mock_render_system()          = default;
        virtual ~mock_render_system() = default;
        MOCK_CONST_METHOD0(monitors, monitor_collection_type());
        MOCK_CONST_METHOD0(primary_monitor, monitor_ref());
        MOCK_METHOD2(create_window,
                     window_ref(const rectangle &, const window_options &));
        MOCK_METHOD0(create_window, window_ref());
        MOCK_METHOD2(locate_shader, asset(shader_type, std::string_view));
    };
} // namespace mge