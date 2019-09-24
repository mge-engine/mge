// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "vulkan.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_system.hpp"
#include "mge/core/log.hpp"
#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif

MGE_USE_LOG(VULKAN);

namespace vulkan {

    class render_system : public mge::render_system
    {
    public:
        render_system()
        {
            MGE_DEBUG_LOG(VULKAN) << "Creating Vulkan render system";
        }

        virtual ~render_system() = default;

        void configure(const mge::configuration& config) override
        {
            m_config.configure(config);
        }

        monitor_collection_t monitors() const override
        {
            return ::platform::monitor::all_monitors();
        }

        mge::monitor_ref primary_monitor() const override
        {
            return platform::monitor::primary_monitor();
        }

        mge::window_ref create_window(const mge::rectangle& rect,
                                      const mge::window_options& options) override
        {
            mge::window_ref result;
            return result;
        }

        system_config m_config;
    };

    MGE_REGISTER_IMPLEMENTATION(render_system,
                                mge::render_system,
                                vulkan);

}