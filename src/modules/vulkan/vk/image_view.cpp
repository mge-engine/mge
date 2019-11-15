#include "image_view.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "error.hpp"

namespace vk {
    image_view::image_view(const device_ref& device, 
                           const surface_ref& surface,
                           VkImage image)
        : m_device(device)
        , m_vk_image_view(VK_NULL_HANDLE)
    {
        VkImageViewCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = image;
        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = surface->format().format;
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;
        auto rc = device->vkCreateImageView(device->vk_device(), &create_info, nullptr, &m_vk_image_view);
        CHECK_VKRESULT(rc, vkCreateImageView);
    }

    image_view::~image_view()
    {
        if (m_vk_image_view && m_device) {
            m_device->vkDestroyImageView(m_device->vk_device(), m_vk_image_view, nullptr);
        }
    }
}