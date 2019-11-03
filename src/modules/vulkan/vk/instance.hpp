#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
namespace vk {

    class instance
    {
    public:
        instance();
        ~instance();
    private:
        VkInstance m_vk_instance;
    };

}
