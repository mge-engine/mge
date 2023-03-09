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

    private:
        void create_surface();
        void create_swap_chain();
        void fetch_surface_capabilities();

        mge::vulkan::render_system& m_render_system;
        mge::vulkan::window&        m_window;

        VkSurfaceKHR                    m_surface;
        VkSurfaceCapabilitiesKHR        m_surface_capabilities;
        std::vector<VkSurfaceFormatKHR> m_surface_formats;
        std::vector<VkPresentModeKHR>   m_surface_present_modes;
    };

} // namespace mge::vulkan
