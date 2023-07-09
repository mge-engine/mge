// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "glslang.hpp"
#include "mge/graphics/render_system.hpp"
#include "vulkan_library.hpp"

#include <map>
#include <optional>
#include <set>

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

        bool debug() const;

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

        inline VkPhysicalDevice physical_device() const noexcept
        {
            return m_physical_device;
        }

        uint32_t graphics_queue_family_index() const;

        const std::vector<VkQueueFamilyProperties>&
        queue_families() const noexcept
        {
            return m_queue_families;
        }

    private:
        void fetch_layers();
        void fetch_instance_extensions(
            const char* layer, std::vector<VkExtensionProperties>& properties);
        void resolve_basic_instance_functions();
        void resolve_instance_functions();
        void create_instance();
        void destroy_instance();
        void teardown();
        void clear_functions();
        void init_debug_messenger();
        void destroy_debug_messenger();
        void pick_physical_device();
        void select_queue_families();
        void select_depth_format();

        static VkBool32
        debug_message_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                               VkDebugUtilsMessageTypeFlagsEXT        type,
                               const VkDebugUtilsMessengerCallbackDataEXT* data,
                               void* userdata);

        std::shared_ptr<vulkan_library>    m_library;
        VkInstance                         m_instance;
        std::vector<VkExtensionProperties> m_instance_extension_properties;
        std::vector<VkLayerProperties>     m_layer_properties;
        std::map<std::string, std::vector<VkExtensionProperties>>
                                      m_layer_extensions;
        VkDebugUtilsMessengerEXT      m_debug_messenger;
        std::vector<VkPhysicalDevice> m_all_physical_devices;
        std::vector<VkPhysicalDeviceProperties>
                                              m_all_physical_device_properties;
        std::vector<VkPhysicalDeviceFeatures> m_all_physical_device_features;

        std::vector<VkQueueFamilyProperties> m_queue_families;
        std::optional<uint32_t>              m_graphics_family;

        VkPhysicalDevice m_physical_device;
        VkQueue          m_graphics_queue;
        VkFormat         m_depth_format;

        std::set<std::string> m_available_extensions;
    };

} // namespace mge::vulkan