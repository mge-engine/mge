#pragma once
#include "vulkan.hpp"
#include "system_config.hpp"
#include "mge/graphics/render_system.hpp"
#include "vulkan_library.hpp"
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

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X;
#define INSTANCE_FUNCTION(X)       PFN_##X X;
#define DEVICE_FUNCTION(X)

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#  include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

    private:
        void check_configure();
        void create_instance();
        void resolve_basic_instance_functions();

        system_config                   m_config;
        std::shared_ptr<vulkan_library> m_library;
    };
}