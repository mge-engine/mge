// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "render_context.hpp"

namespace mge {
    namespace opengl {
        window::window(const mge::extent &        extent,
                       const mge::window_options &options)
            : platform::window(extent, options)
        {
            create_render_context();
        }

        window::~window() {}

        void window::create_render_context()
        {
            m_render_context =
                std::make_shared<mge::opengl::render_context>(this);
        }
    } // namespace opengl
} // namespace mge