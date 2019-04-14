// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "window.hpp"
#include "error.hpp"
#include "mge/core/log.hpp"

MGE_USE_LOG(DX11);

namespace dx11 {

    render_context::render_context(window *w,
                                   const system_config &config)
        : mge::render_context(w)
        ,m_window(w)
    {
        init_context(config);
    }

    render_context::~render_context()
    {}

    void
    render_context::init_context(const system_config& config)
    {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        UINT flags = 0;
#ifdef MGE_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        if(config.debug()) {
            flags |= D3D11_CREATE_DEVICE_DEBUG;
        }
        D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
        if(config.software_device()) {
            driver_type = D3D_DRIVER_TYPE_WARP;
        }
        swap_chain_desc.BufferCount = 1;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.Width = m_window->rect().width();
        swap_chain_desc.BufferDesc.Height = m_window->rect().height();
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.SampleDesc.Count = 4; // TODO: multisampling configurable
        swap_chain_desc.Windowed = TRUE; // TODO: fullscreen
        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.OutputWindow = m_window->hwnd();
        MGE_DEBUG_LOG(DX11) << "Create device and swap chain";

        ID3D11Device *tmp_device;
        ID3D11DeviceContext *tmp_device_context;
        IDXGISwapChain *tmp_swap_chain;

        HRESULT rc = D3D11CreateDeviceAndSwapChain(nullptr, /* adapter */
                                                   driver_type,
                                                   nullptr, /* module */
                                                   flags, /* flags */
                                                   nullptr, // best feature level
                                                   0,
                                                   D3D11_SDK_VERSION,
                                                   &swap_chain_desc,
                                                   &tmp_swap_chain,
                                                   &tmp_device,
                                                   nullptr,
                                                   &tmp_device_context);
        m_device.reset(tmp_device);
        m_device_context.reset(tmp_device_context);
        m_swap_chain.reset(tmp_swap_chain);

        ID3D11Texture2D *tmp_back_buffer = nullptr;
        MGE_DEBUG_LOG(DX11) << "Getting swap chain back buffer";

        m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tmp_back_buffer);
        if (!tmp_back_buffer) {
            MGE_THROW(dx11::error) << "Failed to get back buffer from swap chain";
        }
        COM_UNIQUE_PTR(ID3D11Texture2D) back_buffer;
        back_buffer.reset(tmp_back_buffer);

        MGE_DEBUG_LOG(DX11) << "Creating render target view";
        ID3D11RenderTargetView *tmp_render_target_view;
        rc = m_device->CreateRenderTargetView(back_buffer.get(), nullptr, &tmp_render_target_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateRenderTargetView);
        back_buffer->Release();
        MGE_DEBUG_LOG(DX11) << "Set render target";
        m_device_context->OMSetRenderTargets(1, &tmp_render_target_view, nullptr);
        m_render_target_view.reset(tmp_render_target_view);
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)m_window->rect().width();
        viewport.Height = (float)m_window->rect().height();
        viewport.MinDepth = 0.0;
        viewport.MaxDepth = 0.0;
        MGE_DEBUG_LOG(DX11) << "Set view port";
        m_device_context->RSSetViewports(1, &viewport);
    }

    void
    render_context::flush()
    {
        HRESULT rc = m_swap_chain->Present(0, 0);
        CHECK_HRESULT(rc, IDXGISwapChain, Present);
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::buffer_usage usage,
                                        mge::buffer_access cpu_access,
                                        mge::buffer_access gpu_access)
    {
        mge::index_buffer_ref result;
        return result;
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::buffer_usage usage,
                                        mge::buffer_access cpu_access,
                                        mge::buffer_access gpu_access,
                                        void *data,
                                        size_t size)
    {
        mge::index_buffer_ref result;
        return result;
    }

    void
    render_context::shader_languages(std::vector<mge::shader_language>& languages) const
    {
        return;
    }


}
