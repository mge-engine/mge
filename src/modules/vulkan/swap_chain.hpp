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

    private:
        void create_swap_chain(render_context& context);
        void get_images(render_context& context);
        void create_image_views(render_context& context);

        render_context& vulkan_context();
        void            cleanup();

        VkSwapchainKHR           m_swap_chain;
        std::vector<VkImage>     m_images;
        std::vector<VkImageView> m_image_views;
    };

} // namespace mge::vulkan