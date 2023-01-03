// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#    include "mge/win32/window.hpp"
#else
#    error missing port
#endif

namespace mge::dx12 {

    class render_system;

    class window : public mge::platform::window
    {
    public:
        window(render_system&        system,
               const mge::extent&    extent,
               const window_options& options);
        ~window() = default;

    private:
        void create_render_context();

        render_system& m_render_system;
    };

} // namespace mge::dx12