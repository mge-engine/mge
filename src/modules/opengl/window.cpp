// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);

    namespace opengl {
        window::window(const mge::extent&         extent,
                       const mge::window_options& options)
            : platform::window(extent, options)
        {

            create_render_context();
        }

        window::~window() {}

        void window::create_render_context()
        {
            MGE_DEBUG_TRACE(OPENGL) << "Creating render context";
            m_render_context =
                std::make_shared<mge::opengl::render_context>(this);
        }
    } // namespace opengl
} // namespace mge