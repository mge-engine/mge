#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
#include "system_config.hpp"

namespace vk {

    class instance : public std::enable_shared_from_this<instance>
    {
    public:
        instance(const vulkan::system_config& config);
        ~instance();

        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        PFN_vkDestroyInstance vkDestroyInstance;
        PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;

        PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
        PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
        PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
        PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;

        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

    private:
        template <typename PFN>
        bool resolve_function(PFN& fptr, const char* name);

        void resolve_instance_functions();
        void compute_extensions(std::vector<const char*>& extensions, const vulkan::system_config& config);
        void compute_enabled_layers(const vulkan::system_config& config);
        void setup_debug_callback();
        void fetch_physical_devices();
        void select_physical_device();
        bool physical_device_suitable(size_t index);

        struct physical_device_info
        {
            VkPhysicalDevice           device;
            VkPhysicalDeviceProperties properties;
            VkPhysicalDeviceFeatures   features;
        };

        VkInstance                        m_vk_instance;
        VkDebugUtilsMessengerEXT          m_vk_debug_utils_messenger;
        std::vector<physical_device_info> m_physical_devices;
        std::vector<const char*>          m_enabled_layers;
        size_t                            m_physical_device_index;
        bool                              m_debug;
        bool                              m_debug_utils_found;
        
        
    };

}
