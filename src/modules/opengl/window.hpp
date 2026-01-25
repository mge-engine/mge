// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/config.hpp"
#ifdef MGE_OS_WINDOWS
#    include "mge/win32/window.hpp"
#endif

namespace mge {
    namespace opengl {

        class render_system;

        class window : public platform::window
        {
        public:
            window(mge::opengl::render_system&               system,
                   const ::mge::extent&         extent,
                   const ::mge::window_options& options);
            ~window();

        private:
            void create_render_context(mge::opengl::render_system& system);
        };

    } // namespace opengl
} // namespace mge