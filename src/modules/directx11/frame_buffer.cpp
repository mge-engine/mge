// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "frame_buffer.hpp"
#include "render_context.hpp"
#include "texture.hpp"

namespace mge::dx11 {

    frame_buffer::frame_buffer(render_context& ctx)
        : mge::frame_buffer(ctx)
        , m_context(ctx)
    {}

    frame_buffer::~frame_buffer() = default;

    void frame_buffer::on_attach_color(mge::texture_ref tex, uint32_t slot)
    {
        // RTV is pre-created on the texture; no additional DX11 work at attach time
    }

    void frame_buffer::on_attach_depth(mge::texture_ref tex)
    {
        // DSV is pre-created on the texture; no additional DX11 work at attach time
    }

    ID3D11RenderTargetView* frame_buffer::render_target_view(uint32_t slot) const
    {
        auto tex = color_attachment(slot);
        if (!tex) {
            return nullptr;
        }
        return static_cast<const dx11::texture*>(tex.get())->render_target_view();
    }

    ID3D11DepthStencilView* frame_buffer::depth_stencil_view() const
    {
        auto tex = depth_attachment();
        if (!tex) {
            return nullptr;
        }
        return static_cast<const dx11::texture*>(tex.get())->depth_stencil_view();
    }

} // namespace mge::dx11
