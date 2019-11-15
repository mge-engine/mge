#pragma once
#include "vulkan.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_system.hpp"
#ifdef MGE_OS_WINDOWS
#  include "win32/monitor.hpp"
#endif

namespace vulkan {


    class render_system : public mge::render_system
    {
    public:
        render_system();
        virtual ~render_system() = default;

        void configure(const mge::configuration& config) override;

        monitor_collection_t monitors() const override;
        mge::monitor_ref primary_monitor() const override;
        mge::window_ref create_window(const mge::rectangle& rect,
                                      const mge::window_options& options) override;

    private:
        system_config    m_config;
    };
}