// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "mge/core/stdexceptions.hpp"

namespace mge::vulkan {

    swap_chain::swap_chain(render_context& context)
        : mge::swap_chain(context)
        , m_render_context(context)
    {}

    swap_chain::~swap_chain() {}

    void swap_chain::present()
    {
        m_render_context.present();
    }

    uint32_t swap_chain::current_back_buffer_index() const
    {
        return m_render_context.current_image_index();
    }

    image_ref swap_chain::screenshot()
    {
        MGE_THROW(not_implemented) << "Screenshot not yet implemented";
    }

} // namespace mge::vulkan