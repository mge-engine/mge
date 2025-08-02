// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
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
            MGE_DEBUG_TRACE_STREAM(OPENGL) << "Creating render context";
            auto context = std::make_shared<mge::opengl::render_context>(this);
            context->create_swap_chain();
            m_render_context = context;
        }
    } // namespace opengl
} // namespace mge