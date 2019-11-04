// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/window.hpp"
#ifdef MGE_OS_WINDOWS
#include "win32/window.hpp"
#else
#error missing port
#endif
#include "vk/vulkan.hpp"
#include "system_config.hpp"

namespace vulkan {

    class render_system;

    class window : public platform::window
    {
    public:
        window(render_system& s,
               const mge::rectangle& rect,
               const mge::window_options& options,
               const system_config& config);
        ~window();
    private:
        void create_render_context(const system_config& config);

        render_system& m_render_system;
    };
}
