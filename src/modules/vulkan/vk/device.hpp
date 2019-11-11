#pragma once
#include "mge/config.hpp"
#include "vulkan.hpp"
#include "vk_fwd.hpp"

namespace vk {

    /**
     * Logical device.
     */
    class device
    {
    public:
        device(const instance_ref& instance);
        ~device();

        VkDevice vk_device() const noexcept
        {
            return m_vk_device;
        }


        PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
        PFN_vkDestroyDevice vkDestroyDevice;
    private:
        void resolve_functions();
        template <typename T> T resolve(const char* name);

        instance_ref m_instance;
        VkDevice     m_vk_device;
    };

}