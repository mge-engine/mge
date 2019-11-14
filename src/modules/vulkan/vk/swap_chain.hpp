#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
#include "mge/graphics/extent.hpp"
#include <vector>

namespace vk {


    class swap_chain
    {
    public:
        swap_chain(const device_ref& device,
                   const surface_ref& surface,
                   const mge::extent& base_extent);
        ~swap_chain();
    private:
        
        device_ref           m_device;
        VkSwapchainKHR       m_vk_swap_chain;
        std::vector<VkImage> m_swap_chain_images;
    };
}