// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "error.hpp"
#include "mge/graphics/render_system.hpp"
#include "vulkan.hpp"
#include "vulkan_library.hpp"

namespace mge::vulkan {

    class render_system : public mge::render_system,
                          public std::enable_shared_from_this<render_system>
    {
    public:
        render_system();
        ~render_system();

        mge::window_ref
        create_window(const mge::extent&         extent,
                      const mge::window_options& options) override;

        std::span<mge::monitor_ref> monitors() override;

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define DEVICE_FUNCTION(X) PFN_##X X{nullptr};

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

        bool              debug() const;
        bool              stop_on_validation_errors() const;
        inline VkInstance instance() const noexcept
        {
            return m_instance;
        }
        inline VkPhysicalDevice physical_device() const noexcept
        {
            return m_physical_device;
        }

        inline uint32_t graphics_queue_index() const noexcept
        {
            return static_cast<uint32_t>(m_graphics_queue_index);
        }

        inline uint32_t present_queue_index() const noexcept
        {
            return static_cast<uint32_t>(m_graphics_queue_index);
        }

        inline vulkan_library& library() noexcept
        {
            return *m_library;
        }

    private:
        void init_capabilities();
        static VkBool32
        debug_message_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                               VkDebugUtilsMessageTypeFlagsEXT        type,
                               const VkDebugUtilsMessengerCallbackDataEXT* data,
                               void* userdata);

        void resolve_basic_instance_functions();
        void resolve_layer_properties();
        void create_instance();
        void destroy_instance();
        void resolve_instance_functions();
        void pick_physical_device();
        void select_queue_families();
        void init_glslang();
        void teardown();

        std::shared_ptr<vulkan_library> m_library;
        std::vector<VkLayerProperties>  m_layer_properties;
        std::map<std::string, std::vector<VkExtensionProperties>>
                                      m_instance_extensions;
        VkInstance                    m_instance{VK_NULL_HANDLE};
        VkDebugUtilsMessengerEXT      m_debug_messenger{VK_NULL_HANDLE};
        VkPhysicalDevice              m_physical_device{VK_NULL_HANDLE};
        std::vector<VkPhysicalDevice> m_all_physical_devices;
        std::map<VkPhysicalDevice, VkPhysicalDeviceProperties>
            m_physical_device_properties;
        std::map<VkPhysicalDevice, VkPhysicalDeviceFeatures>
                                             m_physical_device_features;
        std::vector<VkQueueFamilyProperties> m_queue_family_properties;
        size_t                               m_graphics_queue_index{0};
        static std::once_flag                s_glslang_initialized;
    };
} // namespace mge::vulkan