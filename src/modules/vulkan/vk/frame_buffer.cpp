#include "frame_buffer.hpp"
#include "device.hpp"
#include "image_view.hpp"

namespace vk {
    frame_buffer::frame_buffer(const device_ref& device,
                               const image_view_ref& image_view)
        : m_device(device)
        , m_vk_frame_buffer(VK_NULL_HANDLE)
    {
        VkImageView attachments[] = {
            image_view->vk_image_view()
        };
    }

    frame_buffer::~frame_buffer()
    {}
}
