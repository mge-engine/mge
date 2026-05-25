// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx12.hpp"
#include "mge/graphics/frame_buffer.hpp"

namespace mge::dx12 {

    class render_context;

    class frame_buffer : public mge::frame_buffer
    {
    public:
        explicit frame_buffer(render_context& ctx);
        ~frame_buffer() override;

        D3D12_CPU_DESCRIPTOR_HANDLE rtv_cpu_handle(uint32_t slot) const;
        DXGI_FORMAT                 rtv_dxgi_format(uint32_t slot) const;
        D3D12_CPU_DESCRIPTOR_HANDLE dsv_cpu_handle() const;
        DXGI_FORMAT                 dsv_dxgi_format() const;
        bool                        has_depth() const;

    protected:
        void on_attach_color(const mge::texture_ref& tex, uint32_t slot) override;
        void on_attach_depth(const mge::texture_ref& tex) override;

    private:
        render_context& m_context;
    };

} // namespace mge::dx12
