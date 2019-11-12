#pragma once
#include "mge/config.hpp"
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
        PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;

        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
#ifdef MGE_OS_WINDOWS
        PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
#endif
        PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
        PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
        PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
        PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
        PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;

        PFN_vkCreateDevice vkCreateDevice;

        VkInstance vk_instance() const noexcept
        {
            return m_vk_instance;
        }

        uint32_t present_queue_family_index(VkSurfaceKHR surface);

        uint32_t graphics_queue_family_index() const;
        const std::vector<const char*>& enabled_layers() const;
        const std::vector<const char*>& enabled_extensions() const;
        VkPhysicalDevice vk_physical_device() const;
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
            std::vector<VkQueueFamilyProperties> queue_families;
            std::vector<VkExtensionProperties> extensions;
            uint32_t                   graphics_queue_family_index;
            uint32_t                   compute_queue_family_index;
        };

        VkInstance                        m_vk_instance;
        VkDebugUtilsMessengerEXT          m_vk_debug_utils_messenger;
        std::vector<physical_device_info> m_physical_devices;
        std::vector<const char*>          m_enabled_layers;
        std::vector<const char*>          m_enabled_extensions;
        size_t                            m_physical_device_index;
        bool                              m_debug;
        bool                              m_debug_utils_found;
        
        
    };

}
