// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_system.hpp"
#include "mge/core/log.hpp"
#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif
#include "window.hpp"

MGE_USE_LOG(VULKAN);

namespace vulkan {

    render_system::render_system()
    {
        MGE_DEBUG_LOG(VULKAN) << "Creating Vulkan render system";
    }


    void render_system::configure(const mge::configuration& config)
    {
        m_config.configure(config);
    }

    render_system::monitor_collection_t render_system::monitors() const
    {
        return ::platform::monitor::all_monitors();
    }

    mge::monitor_ref render_system::primary_monitor() const
    {
        return platform::monitor::primary_monitor();
    }

    mge::window_ref render_system::create_window(const mge::rectangle& rect,
                                                 const mge::window_options& options) 
    {
        return std::make_shared<window>(*this, rect, options, m_config);
    }


    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                vulkan);

}
