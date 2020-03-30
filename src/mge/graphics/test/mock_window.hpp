// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/window.hpp"
#include "test/googlemock.hpp"

namespace mge {
    class mock_window : public window
    {
    public:
        mock_window()
            : window(mge::rectangle(0, 0, 640, 480),
                     mge::window_options::standard_options())
        {}

        mock_window(const rectangle &dimension, const window_options &options)
            : window(dimension, options)
        {}
        virtual ~mock_window() = default;
        MOCK_METHOD0(on_show, void());
        MOCK_METHOD0(on_hide, void());
        MOCK_METHOD0(request_close, void());
        MOCK_METHOD0(install_display_thread, void());
    };
} // namespace mge
