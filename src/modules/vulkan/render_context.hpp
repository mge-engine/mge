// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "render_context_base.hpp"

#include <atomic>
#include <limits>
#include <vector>

namespace mge::vulkan {
    class render_system;
    class window;

    class render_context : public render_context_base
    {
    public:
        render_context(mge::vulkan::render_system& render_system_,
                       window&                     window_);
        ~render_context();

        void           on_frame_present() override;
        void           render(const mge::pass& p) override;
        mge::image_ref screenshot() override;

        VkQueue present_queue() const noexcept
        {
            return m_queue;
        }

        void present();

        uint32_t current_image_index() const noexcept
        {
            return m_current_image_index;
        }

        VkRenderPass render_pass() const noexcept
        {
            return m_render_pass;
        }

        VkFramebuffer framebuffer(uint32_t index) const
        {
            return m_swap_chain_framebuffers[index];
        }

        VkImage swap_chain_image(uint32_t index) const
        {
            return m_swap_chain_images[index];
        }

        VkExtent2D extent() const noexcept
        {
            return m_extent;
        }

        const VkSurfaceFormatKHR& surface_format() const noexcept
        {
            return m_used_surface_format;
        }

        VkPresentModeKHR present_mode() const noexcept
        {
            return m_used_present_mode;
        }

    protected:
        std::vector<const char*> get_device_extensions() const override;

    private:
        void create_surface();
        void fetch_surface_capabilities();
        void choose_extent();
        void create_swap_chain();
        void create_image_views();
        void create_depth_resources();
        void create_render_pass();
        void create_primary_command_buffers();
        void create_framebuffers();
        void create_fence();
        void create_semaphores();
        void teardown();
        void wait_for_frame_finished();
        void acquire_next_image();

        VkCommandBuffer current_primary_command_buffer() const
        {
            return m_primary_command_buffers[m_current_image_index];
        }

        enum class frame_state
        {
            BEFORE_DRAW,
            DRAW,
            DRAW_FINISHED
        };

        window&                                     m_window;
        VkSurfaceKHR                                m_surface{VK_NULL_HANDLE};
        VkRenderPass             m_render_pass{VK_NULL_HANDLE};
        std::vector<VkSemaphore> m_image_available_semaphores;
        std::vector<VkSemaphore> m_render_finished_semaphores;
        std::vector<VkFence>     m_frame_finished_fences;
        uint32_t                 m_current_frame{0};

        VkSurfaceFormatKHR              m_used_surface_format;
        VkPresentModeKHR                m_used_present_mode;
        VkSurfaceCapabilitiesKHR        m_surface_capabilities;
        std::vector<VkSurfaceFormatKHR> m_surface_formats;
        std::vector<VkPresentModeKHR>   m_surface_present_modes;
        VkExtent2D                      m_extent;
        VkSwapchainKHR                  m_swap_chain_khr{VK_NULL_HANDLE};
        std::vector<VkImage>            m_swap_chain_images;
        std::vector<VkImageView>        m_swap_chain_image_views;
        std::vector<VkImage>            m_depth_images;
        std::vector<VmaAllocation>      m_depth_image_allocations;
        std::vector<VkImageView>        m_depth_image_views;
        std::vector<VkFramebuffer>      m_swap_chain_framebuffers;
        std::vector<VkCommandBuffer>    m_primary_command_buffers;

        std::atomic<uint64_t> m_frame{0};

        uint32_t    m_current_image_index{std::numeric_limits<uint32_t>::max()};
        frame_state m_current_frame_state{frame_state::BEFORE_DRAW};
    };

} // namespace mge::vulkan
