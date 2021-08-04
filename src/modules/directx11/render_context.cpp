// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "render_system.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {
    render_context::render_context(mge::dx11::render_system &render_system_,
                                   mge::dx11::window &       window_)
        : m_render_system(render_system_), m_window(window_)
    {
        init_context();
    }

    render_context::~render_context() {}

    void render_context::init_context()
    {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        UINT flags = (m_render_system.debug() ? D3D11_CREATE_DEVICE_DEBUG : 0);
        D3D_DRIVER_TYPE driver_type =
            (m_render_system.software_device() ? D3D_DRIVER_TYPE_WARP
                                               : D3D_DRIVER_TYPE_HARDWARE);

        ID3D11Device *       tmp_device         = nullptr;
        ID3D11DeviceContext *tmp_device_context = nullptr;

        HRESULT rc = D3D11CreateDevice(
            nullptr, driver_type, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION,
            &tmp_device, nullptr, &tmp_device_context);
        CHECK_HRESULT(rc, , D3D11CreateDevice);

        m_device.reset(tmp_device);
        m_device_context.reset(tmp_device_context);
    }
} // namespace mge::dx11
