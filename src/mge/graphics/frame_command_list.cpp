// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "frame_command_list.hpp"
#include "mge/graphics/render_context.hpp"

namespace mge {
    frame_command_list::frame_command_list(render_context& context,
                                           uint32_t        backbuffer_index,
                                           bool            native)
        : command_list(context, native)
        , m_backbuffer_index(backbuffer_index)
    {}

    frame_command_list::~frame_command_list() {}

    uint32_t frame_command_list::backbuffer_index() const noexcept
    {
        return m_backbuffer_index;
    }

    void frame_command_list::destroy()
    {
        context().destroy_frame_command_list(this);
    }
} // namespace mge