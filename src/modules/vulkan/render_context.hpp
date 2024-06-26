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
    class command_list;
    class swap_chain;

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
        texture_ref       create_texture(texture_type type) override;

        void initialize();

        inline mge::vulkan::render_system& render_system() noexcept
        {
            return m_render_system;
        }

        VkDevice device() const noexcept { return m_device; }

        inline VkSurfaceKHR surface() noexcept { return m_surface; }

        inline VkSurfaceFormatKHR format() noexcept
        {
            return m_used_surface_format;
        }

        inline VkPresentModeKHR present_mode() noexcept
        {
            return m_used_present_mode;
        }

        inline VkCommandPool command_pool() const noexcept
        {
            return m_command_pool;
        }

        VkExtent2D                      extent() const;
        const VkSurfaceCapabilitiesKHR& capabilities() const
        {
            return m_surface_capabilities;
        }

        uint32_t present_queue_family_index() const;

        uint32_t default_image_count() const;

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X;
#define INSTANCE_FUNCTION(X) PFN_##X X;
#define DEVICE_FUNCTION(X) PFN_##X X;

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

        inline VkQueue present_queue() const noexcept
        {
            return m_present_queue;
        }

        inline VkSemaphore render_finished() const noexcept
        {
            return m_render_finished;
        }

        inline VkRenderPass render_pass() const noexcept
        {
            return m_render_pass;
        }

        void begin_draw();
        void end_draw();
        void wait_for_frame_completion();

    private:
        void create_surface();
        void select_present_queue();
        void fetch_surface_capabilities();
        void choose_surface_format();
        void create_device();
        void get_present_queue();
        void get_graphics_queue();
        void resolve_device_functions();
        void create_swap_chain();
        void create_render_pass();
        void create_frame_buffers();
        void create_command_pool();
        void create_fence();
        void create_semaphores();
        void teardown();
        void clear_functions();

        mge::vulkan::render_system& m_render_system;
        mge::vulkan::window&        m_window;

        VkSurfaceKHR                    m_surface;
        VkDevice                        m_device;
        std::optional<uint32_t>         m_present_family;
        VkQueue                         m_graphics_queue;
        VkQueue                         m_present_queue;
        VkSurfaceFormatKHR              m_used_surface_format;
        VkPresentModeKHR                m_used_present_mode;
        VkSurfaceCapabilitiesKHR        m_surface_capabilities;
        std::vector<VkSurfaceFormatKHR> m_surface_formats;
        std::vector<VkPresentModeKHR>   m_surface_present_modes;

        VkRenderPass             m_render_pass;
        VkCommandPool            m_command_pool;
        VkFence                  m_frame_finish;
        VkSemaphore              m_render_finished;
        mge::vulkan::swap_chain* m_vulkan_swap_chain;

        std::vector<command_list*>   m_frame_commands;
        std::vector<VkCommandBuffer> m_frame_command_buffers;
    };

} // namespace mge::vulkan
