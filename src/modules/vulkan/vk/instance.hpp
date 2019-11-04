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

    private:
        template <typename PFN>
        bool resolve_function(PFN& fptr, const char* name);

        void resolve_instance_functions();
        void compute_enabled_layers(const vulkan::system_config& config);

        VkInstance               m_vk_instance;
        std::vector<const char*> m_enabled_layers;
        bool                     m_debug;
        
    };

}
