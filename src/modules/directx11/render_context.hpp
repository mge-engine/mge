// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "render_context_base.hpp"

namespace mge::dx11 {

    class render_system;
    class window;

    /**
     * @brief DX11 render context backed by a window and swap chain.
     */
    class render_context : public render_context_base
    {
    public:
        render_context(mge::dx11::render_system& system, window& window_);
        virtual ~render_context();

        void initialize();

        const ::mge::dx11::window& window() const
        {
            return m_window;
        }

        void setup_context(ID3D11DeviceContext& context);

        void           on_frame_present() override;
        mge::image_ref screenshot() override;

    private:
        mge::dx11::window&                      m_window;
        com_unique_ptr<IDXGISwapChain>          m_swap_chain;
        com_unique_ptr<ID3D11Texture2D>         m_back_buffer;
        com_unique_ptr<ID3D11Texture2D>         m_depth_stencil_buffer;
    };

} // namespace mge::dx11
