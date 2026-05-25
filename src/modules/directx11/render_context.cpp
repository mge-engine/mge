// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "error.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "mge/graphics/memory_image.hpp"
#include "render_system.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {

    render_context::render_context(mge::dx11::render_system& render_system_,
                                   mge::dx11::window&        window_)
        : render_context_base(render_system_, window_.extent())
        , m_window(window_)
    {
        MGE_DEBUG_TRACE(DX11, "Create render context");
    }

    render_context::~render_context()
    {
        error::set_info_queue(nullptr);
    }

    void render_context::initialize()
    {
        MGE_DEBUG_TRACE(DX11, "Initialize render context");

        UINT flags = m_render_system.debug() ? D3D11_CREATE_DEVICE_DEBUG : 0;
        D3D_DRIVER_TYPE driver_type = m_render_system.software_device()
                                          ? D3D_DRIVER_TYPE_WARP
                                          : D3D_DRIVER_TYPE_HARDWARE;

        DXGI_SWAP_CHAIN_DESC swap_chain_desc             = {};
        swap_chain_desc.BufferCount                      = 1;
        swap_chain_desc.BufferDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.Width                 = m_window.extent().width;
        swap_chain_desc.BufferDesc.Height                = m_window.extent().height;
        swap_chain_desc.BufferUsage                      = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.SampleDesc.Count                 = 1;
        swap_chain_desc.Windowed                         = TRUE;
        swap_chain_desc.Flags                            = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.OutputWindow                     = m_window.hwnd();

        ID3D11Device*        tmp_device         = nullptr;
        ID3D11DeviceContext* tmp_device_context = nullptr;
        IDXGISwapChain*      tmp_swap_chain     = nullptr;

        HRESULT rc = D3D11CreateDeviceAndSwapChain(nullptr,
                                                   driver_type,
                                                   nullptr,
                                                   flags,
                                                   nullptr,
                                                   0,
                                                   D3D11_SDK_VERSION,
                                                   &swap_chain_desc,
                                                   &tmp_swap_chain,
                                                   &tmp_device,
                                                   nullptr,
                                                   &tmp_device_context);
        CHECK_HRESULT(rc, , D3D11CreateDeviceAndSwapChain);

        m_device.reset(tmp_device);
        m_device_context.reset(tmp_device_context);
        m_swap_chain.reset(tmp_swap_chain);

        auto fd = m_render_system.frame_debugger();
        if (fd) {
            fd->set_context(frame_debugger::capture_context{m_device.get(),
                                                            m_window.hwnd()});
        }

        if (m_render_system.debug()) {
            MGE_DEBUG_TRACE(DX11, "Enable debug breaks");
            ID3D11Debug* d3d_debug = nullptr;
            m_device->QueryInterface(__uuidof(ID3D11Debug), (void**)&d3d_debug);
            if (d3d_debug) {
                ID3D11InfoQueue* d3d_infoqueue = nullptr;
                if (SUCCEEDED(
                        d3d_debug->QueryInterface(__uuidof(ID3D11InfoQueue),
                                                  (void**)&d3d_infoqueue))) {
                    d3d_infoqueue->SetBreakOnSeverity(
                        D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
                    d3d_infoqueue->SetBreakOnSeverity(
                        D3D11_MESSAGE_SEVERITY_ERROR, true);
                    error::set_info_queue(d3d_infoqueue);
                }
                d3d_debug->Release();
            } else {
                MGE_WARNING_TRACE(DX11, "Cannot enable debug breaks");
            }
        }

        D3D11_TEXTURE2D_DESC depth_buffer_desc      = {};
        depth_buffer_desc.Width                     = window().extent().width;
        depth_buffer_desc.Height                    = window().extent().height;
        depth_buffer_desc.MipLevels                 = 1;
        depth_buffer_desc.ArraySize                 = 1;
        depth_buffer_desc.Format                    = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count          = 1;
        depth_buffer_desc.SampleDesc.Quality        = 0;
        depth_buffer_desc.Usage                     = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags                 = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags            = 0;
        depth_buffer_desc.MiscFlags                 = 0;

        MGE_DEBUG_TRACE(DX11, "Creating depth/stencil buffer texture");
        ID3D11Texture2D* tmp_depth_stencil_buffer;
        rc = m_device->CreateTexture2D(&depth_buffer_desc,
                                       nullptr,
                                       &tmp_depth_stencil_buffer);
        CHECK_HRESULT(rc, ID3D11Device, CreateTexture2D(D3D11_BIND_DEPTH_STENCIL));
        m_depth_stencil_buffer.reset(tmp_depth_stencil_buffer);

        create_default_depth_stencil_state();

        MGE_DEBUG_TRACE(DX11, "Creating back buffer");
        ID3D11Texture2D* tmp_back_buffer = nullptr;
        m_swap_chain->GetBuffer(0,
                                __uuidof(ID3D11Texture2D),
                                reinterpret_cast<LPVOID*>(&tmp_back_buffer));
        if (!tmp_back_buffer) {
            MGE_THROW(dx11::error) << "Failed to get back buffer from swap chain";
        }
        m_back_buffer.reset(tmp_back_buffer);

        MGE_DEBUG_TRACE(DX11, "Creating render target view");
        ID3D11RenderTargetView* tmp_render_target_view;
        rc = m_device->CreateRenderTargetView(m_back_buffer.get(),
                                              nullptr,
                                              &tmp_render_target_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateRenderTargetView);
        m_render_target_view.reset(tmp_render_target_view);

        MGE_DEBUG_TRACE(DX11, "Creating depth stencil view");
        ID3D11DepthStencilView* tmp_depth_stencil_view = nullptr;
        rc = m_device->CreateDepthStencilView(m_depth_stencil_buffer.get(),
                                              nullptr,
                                              &tmp_depth_stencil_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateDepthStencilView);
        m_depth_stencil_view.reset(tmp_depth_stencil_view);

        setup_context(*m_device_context);
        init_capabilities();
    }

    void render_context::setup_context(ID3D11DeviceContext& context)
    {
        ID3D11RenderTargetView* rtv = m_render_target_view.get();
        ID3D11DepthStencilView* dsv = m_depth_stencil_view.get();
        context.OMSetRenderTargets(1, &rtv, dsv);

        context.OMSetDepthStencilState(m_depth_stencil_state.get(), 1);

        D3D11_VIEWPORT viewport  = {};
        viewport.TopLeftX        = 0;
        viewport.TopLeftY        = 0;
        viewport.Width           = static_cast<float>(window().extent().width);
        viewport.Height          = static_cast<float>(window().extent().height);
        viewport.MinDepth        = 0.0;
        viewport.MaxDepth        = 1.0;
        context.RSSetViewports(1, &viewport);
    }

    void render_context::on_frame_present()
    {
        auto rc = m_swap_chain->Present(0, 0);
        CHECK_HRESULT(rc, IDXGISwapChain, Present);
    }

    mge::image_ref render_context::screenshot()
    {
        D3D11_TEXTURE2D_DESC desc;
        m_back_buffer->GetDesc(&desc);

        desc.Usage          = D3D11_USAGE_STAGING;
        desc.BindFlags      = 0;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.MiscFlags      = 0;

        ID3D11Texture2D* staging = nullptr;
        auto rc = m_device->CreateTexture2D(&desc, nullptr, &staging);
        CHECK_HRESULT(rc, ID3D11Device, CreateTexture2D);
        com_unique_ptr<ID3D11Texture2D> staging_ptr(staging);

        m_device_context->CopyResource(staging, m_back_buffer.get());

        D3D11_MAPPED_SUBRESOURCE mapped;
        rc = m_device_context->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
        CHECK_HRESULT(rc, ID3D11DeviceContext, Map);

        mge::image_format fmt(mge::image_format::data_format::RGBA,
                              mge::data_type::UINT8);
        auto              img = std::make_shared<mge::memory_image>(
            fmt, mge::extent(desc.Width, desc.Height));

        uint32_t    row_size = desc.Width * 4;
        auto*       dst      = static_cast<uint8_t*>(img->data());
        const auto* src      = static_cast<const uint8_t*>(mapped.pData);
        for (uint32_t y = 0; y < desc.Height; ++y) {
            memcpy(dst + y * row_size, src + y * mapped.RowPitch, row_size);
        }

        m_device_context->Unmap(staging, 0);
        return img;
    }

} // namespace mge::dx11
