// mge - Modern Game Engine
// Copyright (c) 2018 by Alexander Schroeder
// All rights reserved.
#include "render_context.hpp"
#include "command_list.hpp"
#include "error.hpp"
#include "index_buffer.hpp"
#include "mge/core/log.hpp"
#include "pipeline.hpp"
#include "shader_program.hpp"
#include "texture_2d.hpp"
#include "window.hpp"

MGE_USE_LOG(DX11);

namespace dx11 {

    render_context::render_context(window *w, const system_config &config)
        : mge::render_context(w), m_window(w)
    {
        init_context(config);
    }

    render_context::~render_context() {}

    void render_context::init_context(const system_config &config)
    {
        DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
        UINT                 flags           = 0;
#ifdef MGE_DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        if (config.debug()) {
            flags |= D3D11_CREATE_DEVICE_DEBUG;
        }
        D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
        if (config.software_device()) {
            driver_type = D3D_DRIVER_TYPE_WARP;
        }
        swap_chain_desc.BufferCount       = 1;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.BufferDesc.Width =
            static_cast<UINT>(m_window->rect().width());
        swap_chain_desc.BufferDesc.Height =
            static_cast<UINT>(m_window->rect().height());
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.SampleDesc.Count =
            4;                               // TODO: multisampling configurable
        swap_chain_desc.Windowed     = TRUE; // TODO: fullscreen
        swap_chain_desc.Flags        = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swap_chain_desc.OutputWindow = m_window->hwnd();
        MGE_DEBUG_LOG(DX11) << "Create device and swap chain";

        ID3D11Device *       tmp_device;
        ID3D11DeviceContext *tmp_device_context;
        IDXGISwapChain *     tmp_swap_chain;

        HRESULT rc = D3D11CreateDeviceAndSwapChain(
            nullptr,              /* adapter */
            driver_type, nullptr, /* module */
            flags,                /* flags */
            nullptr,              // best feature level
            0, D3D11_SDK_VERSION, &swap_chain_desc, &tmp_swap_chain,
            &tmp_device, nullptr, &tmp_device_context);
        CHECK_HRESULT(rc, , D3D11CreateDeviceAndSwapChain);
        m_device.reset(tmp_device);
        m_device_context.reset(tmp_device_context);
        m_swap_chain.reset(tmp_swap_chain);
        ID3D11Texture2D *tmp_back_buffer = nullptr;
        MGE_DEBUG_LOG(DX11) << "Getting swap chain back buffer";

        m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                reinterpret_cast<LPVOID *>(&tmp_back_buffer));
        if (!tmp_back_buffer) {
            MGE_THROW(dx11::error)
                << "Failed to get back buffer from swap chain";
        }
        mge::com_unique_ptr<ID3D11Texture2D> back_buffer;
        back_buffer.reset(tmp_back_buffer);

        D3D11_TEXTURE2D_DESC depth_buffer_desc = {};
        depth_buffer_desc.Width                = m_window->rect().width();
        depth_buffer_desc.Height               = m_window->rect().height();
        depth_buffer_desc.MipLevels            = 1;
        depth_buffer_desc.ArraySize            = 1;
        depth_buffer_desc.Format               = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count     = 1;
        depth_buffer_desc.SampleDesc.Quality   = 0;
        depth_buffer_desc.Usage                = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags       = 0;
        depth_buffer_desc.MiscFlags            = 0;

        MGE_DEBUG_LOG(DX11) << "Creating depth buffer texture";
        ID3D11Texture2D *tmp_depth_stencil_buffer;
        rc = m_device->CreateTexture2D(&depth_buffer_desc, nullptr,
                                       &tmp_depth_stencil_buffer);
        CHECK_HRESULT(rc, ID3D11Device,
                      CreateTexture2D(D3D11_BIND_DEPTH_STENCIL));
        m_depth_stencil_buffer.reset(tmp_depth_stencil_buffer);

        MGE_DEBUG_LOG(DX11) << "Creating depth/stencil state";
        D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc = {};
        depth_stencil_state_desc.DepthEnable              = true;
        depth_stencil_state_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depth_stencil_state_desc.DepthFunc      = D3D11_COMPARISON_LESS;

        depth_stencil_state_desc.StencilEnable    = true;
        depth_stencil_state_desc.StencilReadMask  = 0xFF;
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
        ID3D11DepthStencilState *tmp_depth_stencil_state = nullptr;
        rc = m_device->CreateDepthStencilState(&depth_stencil_state_desc,
                                               &tmp_depth_stencil_state);
        CHECK_HRESULT(rc, ID3D11Device, CreateDeptHStencilState);
        m_depth_stencil_state.reset(tmp_depth_stencil_state);

        MGE_DEBUG_LOG(DX11) << "Creating render target view";
        ID3D11RenderTargetView *tmp_render_target_view;
        rc = m_device->CreateRenderTargetView(back_buffer.get(), nullptr,
                                              &tmp_render_target_view);
        CHECK_HRESULT(rc, ID3D11Device, CreateRenderTargetView);
        back_buffer.reset();
        MGE_DEBUG_LOG(DX11) << "Set render target";
        m_device_context->OMSetRenderTargets(1, &tmp_render_target_view,
                                             nullptr);
        m_render_target_view.reset(tmp_render_target_view);
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX       = 0;
        viewport.TopLeftY       = 0;
        viewport.Width          = static_cast<float>(m_window->rect().width());
        viewport.Height         = static_cast<float>(m_window->rect().height());
        viewport.MinDepth       = 0.0;
        viewport.MaxDepth       = 0.0;
        MGE_DEBUG_LOG(DX11) << "Set view port";
        m_device_context->RSSetViewports(1, &viewport);
    }

    void render_context::flush()
    {
        HRESULT rc = m_swap_chain->Present(0, 0);
        CHECK_HRESULT(rc, IDXGISwapChain, Present);
    }

    mge::index_buffer_ref
    render_context::create_index_buffer(mge::data_type type, mge::usage usage,
                                        size_t element_count,
                                        void * initial_data)
    {
        return std::make_shared<index_buffer>(*this, type, usage, element_count,
                                              initial_data);
    }

    mge::vertex_buffer_ref
    render_context::create_vertex_buffer(const mge::vertex_layout &layout,
                                         mge::usage usage, size_t element_count,
                                         void *initial_data)
    {
        mge::vertex_buffer_ref result;
        return result;
    }

    mge::texture_2d_ref
    render_context::create_texture_2d(const mge::image_ref &image)
    {
        return std::make_shared<texture_2d>(*this, image);
    }

    mge::texture_2d_ref render_context::create_texture_2d()
    {
        return std::make_shared<texture_2d>(*this);
    }

    void render_context::shader_languages(
        std::vector<mge::shader_language> &languages) const
    {
        // just use 5.1 for DX11 here
        // https://docs.microsoft.com/en-us/windows/desktop/direct3d11/overviews-direct3d-11-devices-downlevel-intro
        languages.push_back(mge::shader_language("HLSL", mge::version(5, 1)));
    }

    mge::shader_program_ref
    render_context::create_shader_program(mge::shader_type type)
    {
        return std::make_shared<shader_program>(*this, type);
    }

    mge::pipeline_ref render_context::create_pipeline()
    {
        return std::make_shared<pipeline>(*this);
    }

    mge::command_list_ref render_context::create_command_list()
    {
        return std::make_shared<command_list>(*this);
    }

    void render_context::execute(const mge::command_list_ref &commands)
    {
        commands->assert_same_context(*this);
        m_device_context->ExecuteCommandList(
            static_cast<const command_list &>(*commands).native_command_list(),
            TRUE);
    }

} // namespace dx11
