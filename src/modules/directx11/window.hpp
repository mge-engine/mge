// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#include "win32/window.hpp"
#else
#error missing port
#endif
#include "system_config.hpp"

namespace dx11 {
    class window : public ::mge::platform::window
    {
    public:
        window(const mge::rectangle& rect,
               const mge::window_options& options,
               const system_config& config);
        ~window();
    private:
        void create_render_context(const system_config& config);
    };
}
