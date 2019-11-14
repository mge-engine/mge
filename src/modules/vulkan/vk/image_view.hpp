#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
namespace vk {
    class image_view
    {
    public:
        image_view(const device_ref& device,
                   VkImage image);
        ~image_view();
    private:
        device_ref  m_device;
        VkImageView m_vk_image_view;
    };
}