#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
#include "instance.hpp"

namespace vk {


    class physical_device
    {
    public:
        physical_device(VkPhysicalDevice f);
        physical_device(const physical_device& pd);
        ~physical_device() = default;
    private:
        VkPhysicalDevice m_device;
    };
}