// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/render_context.hpp"
namespace mge {
    render_context::render_context(async_executor *display_executor)
        :m_display_executor(display_executor)
    {}

    void
    render_context::await(const std::function<void()>& f)
    {
        m_display_executor->await(f);
    }

    void
    render_context::assign_thread()
    {}

    texture_2d_ref
    render_context::create_texture_2d(usage texture_usage,
                                      filter_function mag_filter,
                                      filter_function min_filter,
                                      bool mipmap_use)
    {
        mge::image_ref nullref;
        return create_texture_2d(texture_usage, mag_filter, min_filter, mipmap_use, nullref);
    }

}
