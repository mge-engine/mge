#pragma once
#include "mge/core/types.hpp"
#include "vulkan.hpp"
#include "vk_fwd.hpp"
#include <vector>

namespace vk {

    /**
     * Vulkan surface. 
     */
    class surface
    {
    public:
#ifdef MGE_OS_WINDOWS
        surface(const instance_ref& instance, HWND hwnd);
#endif
        ~surface();

        VkSurfaceKHR vk_surface() const noexcept
        {
            return m_vk_surface; 
        }
    private:
        instance_ref m_instance;
#ifdef MGE_OS_WINDOWS
        VkSurfaceKHR m_vk_surface;
#endif
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_present_modes;
    };
}