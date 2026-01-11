// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "render_system.hpp"

namespace mge {
    MGE_USE_TRACE(OPENGL);

    namespace opengl {
        window::window(mge::opengl::render_system&             system,
                       const mge::extent&         extent,
                       const mge::window_options& options)
            : platform::window(system, extent, options)
        {

            create_render_context(system);
        }

        window::~window() {}

        void window::create_render_context(mge::opengl::render_system& system)
        {
            MGE_DEBUG_TRACE(OPENGL, "Creating render context");
            auto context =
                std::make_shared<mge::opengl::render_context>(system,
                                                              *this);
            m_render_context = context;
        }
    } // namespace opengl
} // namespace mge