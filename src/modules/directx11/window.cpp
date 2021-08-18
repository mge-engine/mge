// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "render_system.hpp"
#include <memory>

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {
    window::window(render_system&             system,
                   const mge::extent&         extent,
                   const mge::window_options& options)
        : platform::window(extent, options)
        , m_render_system(system)
    {
        MGE_DEBUG_TRACE(DX11) << "Create window";
        create_render_context();
    }

    void window::create_render_context()
    {
        auto context =
            std::make_shared<::mge::dx11::render_context>(m_render_system,
                                                          *this);
        context->initialize();
        m_render_context = context;
    }

} // namespace mge::dx11