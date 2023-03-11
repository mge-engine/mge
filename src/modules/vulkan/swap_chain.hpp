// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/swap_chain.hpp"
#include "vulkan.hpp"

#include <vector>

namespace mge::vulkan {
    class render_context;

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_context& context);
        virtual ~swap_chain();
        void present() override;

        void create_frame_buffers(VkRenderPass render_pass);

        uint32_t next_image();

        inline VkSemaphore image_available() const noexcept
        {
            return m_image_available;
        }

    private:
        void create_swap_chain(render_context& context);
        void get_images(render_context& context);
        void create_image_views(render_context& context);
        void create_semaphore(render_context& context);
        void cleanup();

        render_context& m_vulkan_context;
        VkSwapchainKHR  m_swap_chain;
        VkSemaphore     m_image_available;
        uint32_t        m_current_image;

        std::vector<VkImage>       m_images;
        std::vector<VkImageView>   m_image_views;
        std::vector<VkFramebuffer> m_frame_buffers;
    };

} // namespace mge::vulkan