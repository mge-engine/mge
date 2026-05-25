// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "render_context_base.hpp"

namespace mge::dx12 {
    class render_system;

    class headless_render_context : public render_context_base
    {
    public:
        headless_render_context(mge::dx12::render_system& render_system,
                                const mge::extent&        extent);
        ~headless_render_context() override = default;

        void           on_frame_present() override;
        mge::image_ref screenshot() override;

    protected:
        void setup_backbuffer_as_render_target(
            ID3D12GraphicsCommandList*   cmd_list,
            D3D12_CPU_DESCRIPTOR_HANDLE& pass_rtv,
            D3D12_CPU_DESCRIPTOR_HANDLE& pass_dsv,
            bool&                        pass_has_dsv,
            DXGI_FORMAT&                 pass_rtv_format,
            DXGI_FORMAT&                 pass_dsv_format) override;
    };

} // namespace mge::dx12
