#include "surface.hpp"
#include "instance.hpp"
#include "error.hpp"

#ifdef min
#  undef min
#endif
#ifdef max
#  undef max
#endif

namespace vk {
    surface::surface(const instance_ref& instance, HWND hwnd)
        :m_instance(instance)
#ifdef MGE_OS_WINDOWS
        , m_vk_surface(VK_NULL_HANDLE)
#endif
        , m_present_queue_family_index(std::numeric_limits<uint32_t>::max())
    {
        VkWin32SurfaceCreateInfoKHR create_info = {};
        create_info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        create_info.hinstance = GetModuleHandle(nullptr);
        create_info.hwnd      =  hwnd;

        auto rc = m_instance->vkCreateWin32SurfaceKHR(m_instance->vk_instance(),
                                                      &create_info,
                                                      nullptr /* allocator */,
                                                      &m_vk_surface);
        CHECK_VKRESULT(rc, vkCreateWin32SurfaceKHR);

        rc = m_instance->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_instance->vk_physical_device(),
                                                                   m_vk_surface,
                                                                   &m_capabilities);
        CHECK_VKRESULT(rc, vkGetPhysicalDeviceSurfaceCapabilitiesKHR);

        uint32_t format_count = 0;
        rc = m_instance->vkGetPhysicalDeviceSurfaceFormatsKHR(m_instance->vk_physical_device(),
                                                              m_vk_surface,
                                                              &format_count,
                                                              nullptr);
        CHECK_VKRESULT(rc, vkGetPhysicalDeviceSurfaceFormatsKHR);
        if (format_count != 0) {
            m_formats.resize(format_count);
            rc = m_instance->vkGetPhysicalDeviceSurfaceFormatsKHR(m_instance->vk_physical_device(),
                                                                  m_vk_surface,
                                                                  &format_count,
                                                                  m_formats.data());
            CHECK_VKRESULT(rc, vkGetPhysicalDeviceSurfaceFormatsKHR);
        }

        uint32_t present_modes_count = 0;
        rc = m_instance->vkGetPhysicalDeviceSurfacePresentModesKHR(m_instance->vk_physical_device(),
                                                                   m_vk_surface,
                                                                   &present_modes_count,
                                                                   nullptr);
        CHECK_VKRESULT(rc, vkGetPhysicalDeviceSurfacePresentModesKHR);
        if (present_modes_count != 0) {
            m_present_modes.resize(present_modes_count);
            rc = m_instance->vkGetPhysicalDeviceSurfacePresentModesKHR(m_instance->vk_physical_device(),
                                                                       m_vk_surface,
                                                                       &present_modes_count,
                                                                       m_present_modes.data());   
            CHECK_VKRESULT(rc, vkGetPhysicalDeviceSurfacePresentModesKHR);
        }

        choose_surface_format();
        init_present_queue_family_index();
    }

    surface::~surface()
    {
        if (m_instance && m_vk_surface) {
            m_instance->vkDestroySurfaceKHR(m_instance->vk_instance(),
                                            m_vk_surface,
                                            nullptr /* allocator */);
        }
    }

    void surface::choose_surface_format()
    {
        for (uint32_t i = 0; i < m_formats.size(); ++i) {
            if (m_formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && m_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                m_format_index = i;
                return;
            }
        }

        m_format_index = 0;
    }

    const VkSurfaceFormatKHR& surface::format() const
    {
        return m_formats[m_format_index];
    }

    VkPresentModeKHR surface::present_mode() const
    {
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D surface::extent(const mge::extent& base_extent) const
    {
        if (m_capabilities.currentExtent.width != UINT32_MAX) {
            return m_capabilities.currentExtent;
        } else {
            VkExtent2D e = { base_extent.width(), base_extent.height() };
            e.width = std::max(m_capabilities.minImageExtent.width, 
                               std::min(m_capabilities.maxImageExtent.width, e.width));
            e.height = std::max(m_capabilities.minImageExtent.height, 
                                std::min(m_capabilities.maxImageExtent.height, e.height));
            return e;
        }
    }

    uint32_t surface::min_image_count() const
    {
        return m_capabilities.minImageCount;
    }

    uint32_t surface::max_image_count() const
    {
        return m_capabilities.maxImageCount;
    }

    VkSurfaceTransformFlagBitsKHR surface::current_transform() const
    {
        return m_capabilities.currentTransform;
    }

    void surface::init_present_queue_family_index()
    {
        m_present_queue_family_index 
            = m_instance->present_queue_family_index(vk_surface());
    }

}