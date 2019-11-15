#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
namespace vk {
    class image_view
    {
    public:
        image_view(const device_ref& device,
                   const surface_ref& surface,
                   VkImage image);
        ~image_view();

        VkImageView vk_image_view() const
        {
            return m_vk_image_view;
        }
    private:
        device_ref  m_device;
        VkImageView m_vk_image_view;
    };
}