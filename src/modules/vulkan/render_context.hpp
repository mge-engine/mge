// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "vulkan.hpp"

#include <vector>

namespace mge::vulkan {
    class render_system;
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& system, window& window_);
        ~render_context();

        index_buffer_ref  create_index_buffer(data_type dt,
                                              size_t    data_size,
                                              void*     data) override;
        vertex_buffer_ref create_vertex_buffer(const vertex_layout& layout,
                                               size_t               data_size,
                                               void* data) override;
        shader_ref        create_shader(shader_type t) override;
        program_ref       create_program() override;
        command_list_ref  create_command_list() override;

        void initialize();

        inline mge::vulkan::render_system& render_system() noexcept
        {
            return m_render_system;
        }

        inline VkSurfaceKHR surface() noexcept { return m_surface; }

        inline VkSurfaceFormatKHR format() noexcept
        {
            return m_used_surface_format;
        }

        inline VkPresentModeKHR present_mode() noexcept
        {
            return m_used_present_mode;
        }

        VkExtent2D                      extent() const;
        const VkSurfaceCapabilitiesKHR& capabilities() const
        {
            return m_surface_capabilities;
        }

        uint32_t present_queue_family_index() const;

        uint32_t default_image_count() const;

    private:
        void create_surface();
        void select_present_queue();
        void create_swap_chain();
        void fetch_surface_capabilities();
        void choose_surface_format();
        void create_render_pass();

        mge::vulkan::render_system& m_render_system;
        mge::vulkan::window&        m_window;

        VkSurfaceKHR            m_surface;
        std::optional<uint32_t> m_present_family;
        VkSurfaceFormatKHR      m_used_surface_format;
        VkPresentModeKHR        m_used_present_mode;

        VkSurfaceCapabilitiesKHR        m_surface_capabilities;
        std::vector<VkSurfaceFormatKHR> m_surface_formats;
        std::vector<VkPresentModeKHR>   m_surface_present_modes;

        VkRenderPass m_render_pass;
    };

} // namespace mge::vulkan
