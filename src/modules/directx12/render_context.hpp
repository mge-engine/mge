// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "render_context_base.hpp"
#include "window.hpp"

namespace mge::dx12 {
    class window;
    class render_system;

    class render_context : public render_context_base
    {
    public:
        render_context(mge::dx12::render_system& render_system,
                       window&                   window_);
        ~render_context() override;

        void initialize();

        void on_frame_present() override;

        mge::image_ref screenshot() override;

        const mge::dx12::window& window() const
        {
            return m_window;
        }

        const mge::com_ptr<ID3D12Resource>& backbuffer(uint32_t index) const
        {
            return m_backbuffers[index];
        }

    protected:
        void setup_backbuffer_as_render_target(
            ID3D12GraphicsCommandList*   cmd_list,
            D3D12_CPU_DESCRIPTOR_HANDLE& pass_rtv,
            D3D12_CPU_DESCRIPTOR_HANDLE& pass_dsv,
            bool&                        pass_has_dsv,
            DXGI_FORMAT&                 pass_rtv_format,
            DXGI_FORMAT&                 pass_dsv_format) override;

    private:
        void create_swap_chain();
        void update_render_target_views();
        void create_depth_stencil_views();

        mge::dx12::window&                        m_window;
        std::vector<mge::com_ptr<ID3D12Resource>> m_backbuffers;
        std::vector<mge::com_ptr<ID3D12Resource>> m_depth_buffers;
        mge::com_ptr<IDXGISwapChain4>             m_swap_chain;
    };

} // namespace mge::dx12
