// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "mge/graphics/frame_buffer.hpp"

namespace mge {

    frame_buffer::frame_buffer(render_context& context)
        : mge::context_object(context)
    {}

    void frame_buffer::attach_color(texture_ref tex, uint32_t slot)
    {
        if (slot >= m_color_attachments.size()) {
            m_color_attachments.resize(slot + 1);
        }
        m_color_attachments[slot] = tex;
    }

    void frame_buffer::attach_depth(texture_ref tex)
    {
        m_depth_attachment = tex;
    }

    texture_ref frame_buffer::color_attachment(uint32_t slot) const
    {
        if (slot < m_color_attachments.size()) {
            return m_color_attachments[slot];
        }
        return {};
    }

    texture_ref frame_buffer::depth_attachment() const
    {
        return m_depth_attachment;
    }

} // namespace mge