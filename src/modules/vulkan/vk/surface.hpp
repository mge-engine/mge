#pragma once
#include "mge/core/types.hpp"
#include "mge/graphics/extent.hpp"
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

        const VkSurfaceFormatKHR& format() const;
        VkPresentModeKHR present_mode() const;
        VkExtent2D extent(const mge::extent& base_extent) const;

        uint32_t min_image_count() const;
        uint32_t max_image_count() const;
        VkSurfaceTransformFlagBitsKHR current_transform() const;

        uint32_t present_queue_family_index() const
        {
            return m_present_queue_family_index;
        }

    private:
        void choose_surface_format();
        void init_present_queue_family_index();

        instance_ref m_instance;
#ifdef MGE_OS_WINDOWS
        VkSurfaceKHR m_vk_surface;
#endif
        VkSurfaceCapabilitiesKHR        m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR>   m_present_modes;
        uint32_t                        m_format_index;
        uint32_t                        m_present_queue_family_index;
    };
}