// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "window.hpp"
#include "mge/core/trace.hpp"
#include "render_context.hpp"
#include "render_system.hpp"
#include <memory>

namespace mge {
    MGE_USE_TRACE(DX12);
}

namespace mge::dx12 {
    window::window(render_system&             system,
                   const mge::extent&         extent,
                   const mge::window_options& options)
        : platform::window(system, extent, options)
    {
        MGE_DEBUG_TRACE(DX12, "Create window");
        create_render_context(system);
    }

    void window::create_render_context(render_system& system)
    {
        auto context =
            std::make_shared<::mge::dx12::render_context>(system,
                                                          *this);
        context->initialize();
        m_render_context = context;
    }

} // namespace mge::dx12