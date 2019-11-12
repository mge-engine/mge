#include "surface.hpp"
#include "instance.hpp"
#include "error.hpp"

namespace vk {
    surface::surface(const instance_ref& instance, HWND hwnd)
        :m_instance(instance)
#ifdef MGE_OS_WINDOWS
        , m_vk_surface(VK_NULL_HANDLE)
#endif
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

        MGE_THROW(vulkan::error) << "Found no suitable surface format";
    }

}