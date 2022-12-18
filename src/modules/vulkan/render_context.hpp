// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_system;
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& system, window& window_);
        ~render_context();

    private:
        mge::vulkan::render_system& m_render_system;
        mge::vulkan::window&        m_window;
    };

} // namespace mge::vulkan
