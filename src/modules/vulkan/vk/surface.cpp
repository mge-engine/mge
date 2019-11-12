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

        m_instance->vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_instance->vk_physical_device(),
                                                              m_vk_surface,
                                                              &m_capabilities);

    }

    surface::~surface()
    {
        if (m_instance && m_vk_surface) {
            m_instance->vkDestroySurfaceKHR(m_instance->vk_instance(),
                                            m_vk_surface,
                                            nullptr /* allocator */);
        }
    }

}