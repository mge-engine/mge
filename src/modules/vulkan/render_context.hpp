// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/render_context.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {
    class render_system;
    class window;

    class render_context : public mge::render_context
    {
    public:
        render_context(render_system& render_system_, window& window_);
        ~render_context();

        mge::index_buffer_ref  create_index_buffer(data_type dt,
                                                   size_t    data_size,
                                                   void*     data) override;
        mge::vertex_buffer_ref create_vertex_buffer(const vertex_layout& layout,
                                                    size_t data_size,
                                                    void*  data) override;
        mge::shader_ref        create_shader(shader_type t) override;
        mge::program_ref       create_program() override;
        mge::frame_command_list_ref
                              create_current_frame_command_list() override;
        mge::command_list_ref create_command_list() override;
        mge::texture_ref      create_texture(texture_type type) override;

#define BASIC_INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define INSTANCE_FUNCTION(X) PFN_##X X{nullptr};
#define DEVICE_FUNCTION(X) PFN_##X X{nullptr};

#include "vulkan_core.inc"
#ifdef MGE_OS_WINDOWS
#    include "vulkan_win32.inc"
#endif

#undef BASIC_INSTANCE_FUNCTION
#undef INSTANCE_FUNCTION
#undef DEVICE_FUNCTION

        VkQueue graphics_queue() const noexcept { return m_queue; }
        // TODO: support different present queue
        VkQueue  present_queue() const noexcept { return m_queue; }
        VkDevice device() const noexcept { return m_device; }
        const VkSurfaceFormatKHR& surface_format() const noexcept
        {
            return m_used_surface_format;
        }

        VkPresentModeKHR present_mode() const noexcept
        {
            return m_used_present_mode;
        }

        void present();

        void     init_swap_chain();
        uint32_t current_image_index() const noexcept
        {
            return m_current_image_index;
        }

        VkCommandPool graphics_command_pool() const noexcept
        {
            return m_graphics_command_pool;
        }

    private:
        void create_surface();
        void create_device();
        void create_allocator();
        void get_device_queue();
        void fetch_surface_capabilities();
        void choose_extent();
        void create_swap_chain();
        void create_image_views();
        void create_render_pass();
        void create_graphics_command_pool();
        void create_framebuffers();
        void create_fence();
        void create_semaphores();
        void teardown();
        void resolve_device_functions();
        void clear_functions();

        void tmp_create_command_buffer();
        void tmp_draw_all();

        render_system& m_render_system;
        window&        m_window;
        VkSurfaceKHR   m_surface{VK_NULL_HANDLE};
        VkDevice       m_device{VK_NULL_HANDLE};
        VmaAllocator   m_allocator{VK_NULL_HANDLE};
        VkQueue        m_queue{VK_NULL_HANDLE};
        VkRenderPass   m_render_pass{VK_NULL_HANDLE};
        VkCommandPool  m_graphics_command_pool{VK_NULL_HANDLE};
        VkSemaphore    m_image_available_semaphore{VK_NULL_HANDLE};
        VkSemaphore    m_render_finished_semaphore{VK_NULL_HANDLE};
        VkFence        m_frame_finished_fence{VK_NULL_HANDLE};
        uint32_t       m_current_image_index{0};

        VkSurfaceFormatKHR              m_used_surface_format;
        VkPresentModeKHR                m_used_present_mode;
        VkSurfaceCapabilitiesKHR        m_surface_capabilities;
        std::vector<VkSurfaceFormatKHR> m_surface_formats;
        std::vector<VkPresentModeKHR>   m_surface_present_modes;
        VkExtent2D                      m_extent;
        VkSwapchainKHR                  m_swap_chain_khr{VK_NULL_HANDLE};
        std::vector<VkImage>            m_swap_chain_images;
        std::vector<VkImageView>        m_swap_chain_image_views;
        std::vector<VkFramebuffer>      m_swap_chain_framebuffers;

        VkCommandBuffer m_tmp_command_buffer;
    };
} // namespace mge::vulkan