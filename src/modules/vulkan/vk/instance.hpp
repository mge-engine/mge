#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
#include "system_config.hpp"

namespace vk {

    class instance
    {
    public:
        instance(const vulkan::system_config& config);
        ~instance();

        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        PFN_vkDestroyInstance vkDestroyInstance;
        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
    private:
        template <typename PFN>
        bool resolve_function(PFN& fptr, const char* name);

        void resolve_instance_functions();
        void compute_extensions(std::vector<const char*>& extensions, const vulkan::system_config& config);
        void compute_enabled_layers(const vulkan::system_config& config);
        void setup_debug_callback();

        VkInstance               m_vk_instance;
        VkDebugUtilsMessengerEXT m_vk_debug_utils_messenger;
        std::vector<const char*> m_enabled_layers;
        bool                     m_debug;
        bool                     m_debug_utils_found;
        
        
    };

}
