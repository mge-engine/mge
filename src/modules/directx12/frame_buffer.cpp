// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "frame_buffer.hpp"
#include "render_context.hpp"
#include "texture.hpp"

namespace mge::dx12 {

    frame_buffer::frame_buffer(render_context& ctx)
        : mge::frame_buffer(ctx)
        , m_context(ctx)
    {}

    frame_buffer::~frame_buffer() = default;

    void frame_buffer::on_attach_color(mge::texture_ref tex, uint32_t slot)
    {
        // RTV and view format are pre-created on the texture
    }

    void frame_buffer::on_attach_depth(mge::texture_ref tex)
    {
        // DSV and view format are pre-created on the texture
    }

    D3D12_CPU_DESCRIPTOR_HANDLE
    frame_buffer::rtv_cpu_handle(uint32_t slot) const
    {
        auto tex = color_attachment(slot);
        if (!tex) {
            return {};
        }
        return static_cast<const dx12::texture*>(tex.get())->rtv_cpu_handle();
    }

    DXGI_FORMAT frame_buffer::rtv_dxgi_format(uint32_t slot) const
    {
        auto tex = color_attachment(slot);
        if (!tex) {
            return DXGI_FORMAT_UNKNOWN;
        }
        return static_cast<const dx12::texture*>(tex.get())->view_dxgi_format();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE frame_buffer::dsv_cpu_handle() const
    {
        auto tex = depth_attachment();
        if (!tex) {
            return {};
        }
        return static_cast<const dx12::texture*>(tex.get())->dsv_cpu_handle();
    }

    DXGI_FORMAT frame_buffer::dsv_dxgi_format() const
    {
        auto tex = depth_attachment();
        if (!tex) {
            return DXGI_FORMAT_UNKNOWN;
        }
        return static_cast<const dx12::texture*>(tex.get())->view_dxgi_format();
    }

    bool frame_buffer::has_depth() const
    {
        return depth_attachment() != nullptr;
    }

} // namespace mge::dx12
