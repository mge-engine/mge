#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
#include "mge/graphics/extent.hpp"
#include "mge/core/small_vector.hpp"

namespace vk {


    class swap_chain
    {
    public:
        using image_vector = mge::small_vector<VkImage, 3>;

        swap_chain(const device_ref& device,
                   const surface_ref& surface,
                   const mge::extent& base_extent);
        ~swap_chain();

        const image_vector& images() const
        {
            return m_images;
        }
    private:
        
        device_ref      m_device;
        VkSwapchainKHR  m_vk_swap_chain;
        image_vector    m_images;
    };
}