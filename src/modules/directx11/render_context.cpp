// mge - Modern Game Engine
// Copyright (c) 2021 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/trace.hpp"
#include "render_system.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {
    render_context::render_context(mge::dx11::render_system& render_system_,
                                   mge::dx11::window&        window_)
        : m_render_system(render_system_)
        , m_window(window_)
    {
        MGE_DEBUG_TRACE(DX11) << "Create render context";
    }

    render_context::~render_context() {}

    void render_context::initialize()
    {
        MGE_DEBUG_TRACE(DX11) << "Initialize render context";
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        UINT flags = (m_render_system.debug() ? D3D11_CREATE_DEVICE_DEBUG : 0);
        D3D_DRIVER_TYPE driver_type =
            (m_render_system.software_device() ? D3D_DRIVER_TYPE_WARP
                                               : D3D_DRIVER_TYPE_HARDWARE);

        swap_chain_desc.BufferCount = 1;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.Width = m_window.extent().width;
        swap_chain_desc.BufferDesc.Height = m_window.extent().height;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.SampleDesc.Count =
            4; // TODO: #121 multisampling configurable
        swap_chain_desc.Windowed = TRUE; // TODO: #122 fullscreen

        swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.OutputWindow = m_window.hwnd();

        ID3D11Device*        tmp_device = nullptr;
        ID3D11DeviceContext* tmp_device_context = nullptr;
        IDXGISwapChain*      tmp_swap_chain = nullptr;

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
        auto swap_chain =
            std::make_shared<mge::dx11::swap_chain>(*this, tmp_swap_chain);
        m_swap_chain = swap_chain;

        MGE_DEBUG_TRACE(DX11) << "Creating render target view";

        ID3D11RenderTargetView* tmp_render_target_view;

        auto back_buffer = swap_chain->back_buffer();
        rc = m_device->CreateRenderTargetView(back_buffer.get(),
                                              nullptr,
                                              &tmp_render_target_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateRenderTargetView);
        back_buffer.reset();
        MGE_DEBUG_TRACE(DX11) << "Set render target";
        m_device_context->OMSetRenderTargets(1,
                                             &tmp_render_target_view,
                                             nullptr);
        m_render_target_view.reset(tmp_render_target_view);
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<float>(window().extent().width);
        viewport.Height = static_cast<float>(window().extent().height);
        viewport.MinDepth = 0.0;
        viewport.MaxDepth = 0.0;
        MGE_DEBUG_TRACE(DX11) << "Set view port";
        m_device_context->RSSetViewports(1, &viewport);
    }

    mge::index_buffer_ref render_context::create_index_buffer(mge::data_type dt,
                                                              size_t data_size,
                                                              void*  data)
    {
        // MGE_DEBUG_TRACE(DX11) << "Create index buffer";
        mge::index_buffer_ref result =
            std::make_shared<index_buffer>(*this, dt, data_size, data);
        return result;
    }

    mge::vertex_buffer_ref render_context::create_vertex_buffer(
        const mge::vertex_layout& layout, size_t data_size, void* data)
    {
        mge::vertex_buffer_ref result;
        return result;
    }

    mge::shader_ref render_context::create_shader(mge::shader_type t)
    {
        mge::shader_ref result;
        return result;
    }

    mge::command_list_ref render_context::create_command_list()
    {
        auto result = std::make_shared<mge::dx11::command_list>(*this);
        return result;
    }

} // namespace mge::dx11
