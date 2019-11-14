#include "swap_chain.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(VULKAN);

namespace vk {
    swap_chain::swap_chain(const device_ref& device,
                           const surface_ref& surface,
                           const mge::extent& base_extent)
        :m_device(device)
        ,m_vk_swap_chain(VK_NULL_HANDLE)
    {
        VkSwapchainCreateInfoKHR create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = surface->vk_surface();
        create_info.minImageCount = surface->min_image_count() + 1;
        if (create_info.minImageCount > surface->max_image_count()) {
            create_info.minImageCount = surface->max_image_count();
        }
        create_info.imageFormat = surface->format().format;
        create_info.imageColorSpace = surface->format().colorSpace;
        create_info.imageExtent = surface->extent(base_extent);
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        uint32_t queue_family_indices[] = { device->graphics_queue_family_index(), surface->present_queue_family_index() };
        if (queue_family_indices[0] != queue_family_indices[1]) {
            MGE_DEBUG_LOG(VULKAN) << "Swap chain has different graphics and present queue";
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        } else {
            MGE_DEBUG_LOG(VULKAN) << "Swap chain uses same queue as graphics and present queue";
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0;
            create_info.pQueueFamilyIndices = nullptr;
        }
        create_info.preTransform = surface->current_transform();
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = surface->present_mode();
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = VK_NULL_HANDLE;

        auto rc = device->vkCreateSwapchainKHR(device->vk_device(), &create_info, nullptr, &m_vk_swap_chain);
        CHECK_VKRESULT(rc, vkCreateSwapchainKHR);
    }

    swap_chain::~swap_chain()
    {
        if (m_vk_swap_chain && m_device) {
            m_device->vkDestroySwapchainKHR(m_device->vk_device(), m_vk_swap_chain, nullptr);
        }
    }
}