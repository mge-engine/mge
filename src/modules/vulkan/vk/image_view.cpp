#include "image_view.hpp"
namespace vk {
    image_view::image_view(const device_ref& device, VkImage image)
        : m_device(device)
        , m_vk_image_view(VK_NULL_HANDLE)
    {}

    image_view::~image_view()
    {}
}