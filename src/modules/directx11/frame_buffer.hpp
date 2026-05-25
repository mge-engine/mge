// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "dx11.hpp"
#include "mge/graphics/frame_buffer.hpp"
#include "mge/win32/com_unique_ptr.hpp"

namespace mge::dx11 {

    class render_context;

    class frame_buffer : public mge::frame_buffer
    {
    public:
        explicit frame_buffer(render_context& ctx);
        ~frame_buffer() override;

        ID3D11RenderTargetView* render_target_view(uint32_t slot) const;
        ID3D11DepthStencilView* depth_stencil_view() const;

    protected:
        void on_attach_color(const mge::texture_ref& tex, uint32_t slot) override;
        void on_attach_depth(const mge::texture_ref& tex) override;

    private:
        render_context& m_context;
    };

} // namespace mge::dx11
