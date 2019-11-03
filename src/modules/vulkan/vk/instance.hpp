#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
namespace vk {

    class instance
    {
    public:
        instance(bool debug);
        ~instance();

        PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
        PFN_vkDestroyInstance vkDestroyInstance;

    private:
        template <typename PFN>
        bool resolve_function(PFN& fptr, const char* name);

        void resolve_instance_functions();

        VkInstance m_vk_instance;
        bool       m_debug;
    };

}
