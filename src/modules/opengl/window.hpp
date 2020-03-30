// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/window.hpp"
#ifdef MGE_OS_WINDOWS
#    include "win32/window.hpp"
#else
#    error missing port
#endif
#include "opengl.hpp"

namespace opengl {
    class window : public ::mge::platform::window
    {
    public:
        window(const mge::rectangle &rect, const mge::window_options &options,
               bool debug);
        ~window();

    private:
        void create_render_context(bool debug);
    };
} // namespace opengl
