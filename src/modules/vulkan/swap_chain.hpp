// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "mge/graphics/swap_chain.hpp"
#include "render_context.hpp"
#include "vulkan.hpp"

namespace mge::vulkan {

    class swap_chain : public mge::swap_chain
    {
    public:
        swap_chain(render_context& context);
        virtual ~swap_chain();
        void      present() override;
        uint32_t  current_back_buffer_index() const override;
        image_ref screenshot() override;

    private:
        render_context& m_render_context;
    };

} // namespace mge::vulkan