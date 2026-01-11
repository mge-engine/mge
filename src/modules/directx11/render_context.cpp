// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/configuration.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/frame_debugger.hpp"
#include "program.hpp"
#include "render_system.hpp"
#include "shader.hpp"
#include "swap_chain.hpp"
#include "texture.hpp"
#include "vertex_buffer.hpp"
#include "window.hpp"
namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {
    render_context::render_context(mge::dx11::render_system& render_system_,
                                   mge::dx11::window&        window_)
        : mge::render_context(render_system_, window_.extent())
        , m_render_system(render_system_)
        , m_window(window_)
    {
        MGE_DEBUG_TRACE(DX11, "Create render context");
    }

    render_context::~render_context()
    {
        if (m_render_system.frame_debugger()) {
            auto fd = m_render_system.frame_debugger();
            if (fd) {
                MGE_INFO_TRACE(DX11, "Ending frame recording");
                fd->end_capture();
            }
        }
    }

    void render_context::initialize()
    {
        MGE_DEBUG_TRACE(DX11, "Initialize render context");
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
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.Windowed = TRUE;

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

        auto fd = m_render_system.frame_debugger();
        if (fd) {
            fd->set_context(frame_debugger::capture_context{m_device.get(),
                                                            m_window.hwnd()});
        }

        try {
            m_record_frames = std::any_cast<bool>(
                configuration::get("graphics", "record_frames").value());
            if (m_record_frames) {
                MGE_INFO_TRACE(DX11, "Frame recording is enabled");
            } else {
                MGE_INFO_TRACE(DX11, "Frame recording is disabled");
            }
        } catch (const mge::exception& e) {
            MGE_WARNING_TRACE(DX11,
                              "Error reading frame recording configuration: {}",
                              e.what());
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
                        D3D11_MESSAGE_SEVERITY_CORRUPTION,
                        true);
                    d3d_infoqueue->SetBreakOnSeverity(
                        D3D11_MESSAGE_SEVERITY_ERROR,
                        true);
                    d3d_infoqueue->Release();
                }
                d3d_debug->Release();
            } else {
                MGE_WARNING_TRACE(DX11, "Cannot enable debug breaks");
            }
        }
        MGE_DEBUG_TRACE(DX11, "Creating swap chain");
        m_swap_chain.reset(tmp_swap_chain);

        D3D11_TEXTURE2D_DESC depth_buffer_desc = {};
        depth_buffer_desc.Width = window().extent().width;
        depth_buffer_desc.Height = window().extent().height;
        depth_buffer_desc.MipLevels = 1;
        depth_buffer_desc.ArraySize = 1;
        depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count = 1;
        depth_buffer_desc.SampleDesc.Quality = 0;
        depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags = 0;
        depth_buffer_desc.MiscFlags = 0;

        MGE_DEBUG_TRACE(DX11, "Creating depth/stencil buffer texture");
        ID3D11Texture2D* tmp_depth_stencil_buffer;

        rc = m_device->CreateTexture2D(&depth_buffer_desc,
                                       nullptr,
                                       &tmp_depth_stencil_buffer);
        CHECK_HRESULT(rc,
                      ID3D11Device,
                      CreateTexture2D(D3D11_BIND_DEPTH_STENCIL));
        m_depth_stencil_buffer.reset(tmp_depth_stencil_buffer);

        MGE_DEBUG_TRACE(DX11, "Creating depth/stencil state");
        D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc = {};
        depth_stencil_state_desc.DepthEnable = true;
        depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_stencil_state_desc.DepthFunc = D3D11_COMPARISON_LESS;

        depth_stencil_state_desc.StencilEnable = true;
        depth_stencil_state_desc.StencilReadMask = 0xFF;
        depth_stencil_state_desc.StencilWriteMask = 0xFF;

        // Stencil operations if pixel is front-facing.
        depth_stencil_state_desc.FrontFace.StencilFailOp =
            D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.FrontFace.StencilDepthFailOp =
            D3D11_STENCIL_OP_INCR;
        depth_stencil_state_desc.FrontFace.StencilPassOp =
            D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.FrontFace.StencilFunc =
            D3D11_COMPARISON_ALWAYS;

        // Stencil operations if pixel is back-facing.
        depth_stencil_state_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.BackFace.StencilDepthFailOp =
            D3D11_STENCIL_OP_DECR;
        depth_stencil_state_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depth_stencil_state_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        ID3D11DepthStencilState* tmp_depth_stencil_state = nullptr;
        rc = m_device->CreateDepthStencilState(&depth_stencil_state_desc,
                                               &tmp_depth_stencil_state);
        CHECK_HRESULT(rc, ID3D11Device, CreateDeptHStencilState);
        m_depth_stencil_state.reset(tmp_depth_stencil_state);

        MGE_DEBUG_TRACE(DX11, "Creating back buffer");
        ID3D11Texture2D* tmp_back_buffer = nullptr;
        m_swap_chain->GetBuffer(0,
                                __uuidof(ID3D11Texture2D),
                                reinterpret_cast<LPVOID*>(&tmp_back_buffer));
        if (!tmp_back_buffer) {
            MGE_THROW(dx11::error)
                << "Failed to get back buffer from swap chain";
        }
        m_back_buffer.reset(tmp_back_buffer);

        MGE_DEBUG_TRACE(DX11, "Creating render target view");
        ID3D11RenderTargetView* tmp_render_target_view;
        rc = m_device->CreateRenderTargetView(m_back_buffer.get(),
                                              nullptr,
                                              &tmp_render_target_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateRenderTargetView);

        MGE_DEBUG_TRACE(DX11, "Set render target");
        m_render_target_view.reset(tmp_render_target_view);

        MGE_DEBUG_TRACE(DX11, "Creating depth stencil view");
        ID3D11DepthStencilView* tmp_depth_stencil_view = nullptr;
        rc = m_device->CreateDepthStencilView(m_depth_stencil_buffer.get(),
                                              nullptr,
                                              &tmp_depth_stencil_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateDepthStencilView);
        m_depth_stencil_view.reset(tmp_depth_stencil_view);

        setup_context(*m_device_context);
    }

    void render_context::setup_context(ID3D11DeviceContext& context)
    {
        ID3D11RenderTargetView* rtv = m_render_target_view.get();
        ID3D11DepthStencilView* dsv = m_depth_stencil_view.get();
        context.OMSetRenderTargets(1, &rtv, dsv);

        context.OMSetDepthStencilState(m_depth_stencil_state.get(), 1);

        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<float>(window().extent().width);
        viewport.Height = static_cast<float>(window().extent().height);
        viewport.MinDepth = 0.0;
        viewport.MaxDepth = 1.0;
        context.RSSetViewports(1, &viewport);
    }

    mge::index_buffer* render_context::on_create_index_buffer(mge::data_type dt,
                                                              size_t data_size)
    {
        return new index_buffer(*this, dt, data_size);
    }

    mge::vertex_buffer*
    render_context::on_create_vertex_buffer(const mge::vertex_layout& layout,
                                            size_t                    data_size)
    {
        return new mge::dx11::vertex_buffer(*this, layout, data_size);
    }

    mge::shader* render_context::on_create_shader(mge::shader_type t)
    {
        auto result = new shader(*this, t);
        return result;
    }

    mge::program* render_context::on_create_program()
    {
        return new program(*this);
    }

    mge::texture_ref render_context::create_texture(mge::texture_type type)
    {
        mge::texture_ref result = std::make_shared<texture>(*this, type);
        return result;
    }

    mge::rectangle render_context::default_scissor() const
    {
        return mge::rectangle(m_window.position(), m_window.extent());
    }

    void render_context::render(const mge::pass& p)
    {
        if (m_first_frame) {
            m_first_frame = false;
            if (m_record_frames) {
                auto fd = m_render_system.frame_debugger();
                if (fd) {
                    MGE_INFO_TRACE(DX11, "Starting frame recording");
                    fd->begin_capture();
                }
            }
        }

        ID3D11RenderTargetView* rtv = nullptr;
        if (p.frame_buffer()) {
            // support custom frame buffers
        } else {
            rtv = m_render_target_view.get();
        }

        if (p.clear_color_enabled()) {
            float clearcolor[4] = {p.clear_color_value().r,
                                   p.clear_color_value().g,
                                   p.clear_color_value().b,
                                   p.clear_color_value().a};
            m_device_context->ClearRenderTargetView(rtv, clearcolor);
        }
    }

    mge::image_ref render_context::screenshot()
    {
        return mge::image_ref();
    }

    void render_context::on_frame_present()
    {
        auto rc = m_swap_chain->Present(0, 0);
        CHECK_HRESULT(rc, IDXGISwapChain, Present);
    }
} // namespace mge::dx11
