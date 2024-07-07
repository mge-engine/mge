// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "error.hpp"
#include "mge/graphics/render_system.hpp"
#include "vulkan.hpp"
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

        std::span<mge::monitor_ref> monitors() override;

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

        inline VkInstance instance() const noexcept { return m_instance; }

    private:
        void resolve_basic_instance_functions();
        void resolve_layer_properties();
        void teardown();

        std::shared_ptr<vulkan_library> m_library;
        std::vector<VkLayerProperties>  m_layer_properties;
        std::map<std::string, std::vector<VkExtensionProperties>>
                   m_instance_extensions;
        VkInstance m_instance{VK_NULL_HANDLE};
    };
} // namespace mge::vulkan