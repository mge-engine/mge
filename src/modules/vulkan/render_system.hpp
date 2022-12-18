// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_system.hpp"
#include "vulkan_library.hpp"

namespace mge::vulkan {

    class render_system : public mge::render_system
    {
    public:
        render_system();
        ~render_system();

        mge::window_ref
        create_window(const mge::extent&         extent,
                      const mge::window_options& options) override;

        mge::render_system::monitor_collection monitors() override;

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X;
#define INSTANCE_FUNCTION(X) PFN_##X X;
#define DEVICE_FUNCTION(X) PFN_##X X;

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

    private:
        void fetch_layers();
        void fetch_instance_extensions(const char* layer);
        void resolve_basic_instance_functions();
        void resolve_instance_functions();
        void create_instance();
        void destroy_instance();
        void teardown();
        void clear_functions();

        std::shared_ptr<vulkan_library> m_library;
        VkInstance                      m_instance;
    };

} // namespace mge::vulkan