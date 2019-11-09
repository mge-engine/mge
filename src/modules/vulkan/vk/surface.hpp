#pragma once
#include "mge/core/types.hpp"
#include "vulkan.hpp"
#include "vk_fwd.hpp"

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
    private:
        instance_ref m_instance;
#ifdef MGE_OS_WINDOWS
        VkSurfaceKHR m_vk_surface;
#endif
    };
}