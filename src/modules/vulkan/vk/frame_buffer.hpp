#pragma once
#include "vulkan.hpp"
#include "vk_fwd.hpp"
namespace vk {
    class frame_buffer
    {
    public:
        frame_buffer(const device_ref& device,
                     const image_view_ref& image_view);
        ~frame_buffer();

    private:
        device_ref    m_device;
        VkFramebuffer m_vk_frame_buffer;
    };
}