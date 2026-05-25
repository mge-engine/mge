// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#pragma once
#include "render_context_base.hpp"

namespace mge::dx11 {

    class render_system;

    /**
     * @brief DX11 render context without a window or swap chain.
     *
     * The device is created with @c D3D11CreateDevice (no HWND required).
     * Rendering must target a @c frame_buffer; @c screenshot() reads back
     * the first colour attachment of the first populated frame buffer.
     * @c on_frame_present() is a no-op.
     */
    class headless_render_context : public render_context_base
    {
    public:
        headless_render_context(mge::dx11::render_system& system,
                                const mge::extent&        ext);
        ~headless_render_context() = default;

        void           initialize();
        void           on_frame_present() override;
        mge::image_ref screenshot() override;
    };

} // namespace mge::dx11
