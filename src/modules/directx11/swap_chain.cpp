// mge - Modern Game Engine
// Copyright (c) 2017-2023 by Alexander Schroeder
// All rights reserved.
#include "swap_chain.hpp"
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/trace.hpp"
#include "mge/graphics/memory_image.hpp"
#include "window.hpp"

namespace mge {
    MGE_USE_TRACE(DX11);
}

namespace mge::dx11 {
    swap_chain::swap_chain(render_context& context, IDXGISwapChain* swap_chain)
        : mge::swap_chain(context)
    {
        m_swap_chain.reset(swap_chain);

        D3D11_TEXTURE2D_DESC depth_buffer_desc = {};
        depth_buffer_desc.Width = context.window().extent().width;
        depth_buffer_desc.Height = context.window().extent().height;
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

        auto rc = context.device()->CreateTexture2D(&depth_buffer_desc,
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
        rc =
            context.device()->CreateDepthStencilState(&depth_stencil_state_desc,
                                                      &tmp_depth_stencil_state);
        CHECK_HRESULT(rc, ID3D11Device, CreateDeptHStencilState);
        m_depth_stencil_state.reset(tmp_depth_stencil_state);
    }

    com_unique_ptr<ID3D11Texture2D> swap_chain::back_buffer() const
    {
        ID3D11Texture2D* tmp_back_buffer = nullptr;
        m_swap_chain->GetBuffer(0,
                                __uuidof(ID3D11Texture2D),
                                reinterpret_cast<LPVOID*>(&tmp_back_buffer));
        if (!tmp_back_buffer) {
            MGE_THROW(dx11::error)
                << "Failed to get back buffer from swap chain";
        }
        com_unique_ptr<ID3D11Texture2D> back_buffer;
        back_buffer.reset(tmp_back_buffer);
        return back_buffer;
    }

    swap_chain::~swap_chain() {}

    void swap_chain::present()
    {
        auto hr = m_swap_chain->Present(0, 0);
        CHECK_HRESULT(hr, IDXGISwapChain, Present);
    }

    image_ref swap_chain::screenshot()
    {
        auto& rc = static_cast<render_context&>(context());
        auto  extent = rc.extent();

        // Get the back buffer
        auto back_buffer_texture = back_buffer();

        // Get the description of the back buffer
        D3D11_TEXTURE2D_DESC back_buffer_desc;
        back_buffer_texture->GetDesc(&back_buffer_desc);

        // Create a staging texture for CPU access
        D3D11_TEXTURE2D_DESC staging_desc = back_buffer_desc;
        staging_desc.Usage = D3D11_USAGE_STAGING;
        staging_desc.BindFlags = 0;
        staging_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        staging_desc.MiscFlags = 0;

        ID3D11Texture2D* tmp_staging_texture = nullptr;
        auto             hr = rc.device()->CreateTexture2D(&staging_desc,
                                               nullptr,
                                               &tmp_staging_texture);
        CHECK_HRESULT(hr, ID3D11Device, CreateTexture2D(staging));

        com_unique_ptr<ID3D11Texture2D> staging_texture;
        staging_texture.reset(tmp_staging_texture);

        // Copy the back buffer to the staging texture
        rc.device_context()->CopyResource(staging_texture.get(),
                                          back_buffer_texture.get());

        // Map the staging texture to read pixel data
        D3D11_MAPPED_SUBRESOURCE mapped_resource;
        hr = rc.device_context()->Map(staging_texture.get(),
                                      0,
                                      D3D11_MAP_READ,
                                      0,
                                      &mapped_resource);
        CHECK_HRESULT(hr, ID3D11DeviceContext, Map);

        // Create the image with RGBA format
        image_format format(image_format::data_format::RGBA, data_type::UINT8);
        auto         img =
            std::make_shared<memory_image>(format,
                                           extent,
                                           extent.width * extent.height * 4);

        // Copy pixel data - check if we need to swap channels based on format
        auto* dest = static_cast<uint8_t*>(img->data());
        auto* src = static_cast<uint8_t*>(mapped_resource.pData);

        // DirectX 11 typically uses BGRA format, but let's check
        bool need_swap =
            (back_buffer_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM ||
             back_buffer_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);

        for (uint32_t y = 0; y < extent.height; ++y) {
            for (uint32_t x = 0; x < extent.width; ++x) {
                size_t src_offset = y * mapped_resource.RowPitch + x * 4;
                size_t dest_offset = (y * extent.width + x) * 4;

                if (need_swap) {
                    // Convert BGRA to RGBA
                    dest[dest_offset + 0] = src[src_offset + 2]; // R from B
                    dest[dest_offset + 1] = src[src_offset + 1]; // G
                    dest[dest_offset + 2] = src[src_offset + 0]; // B from R
                    dest[dest_offset + 3] = src[src_offset + 3]; // A
                } else {
                    // Already RGBA, just copy
                    dest[dest_offset + 0] = src[src_offset + 0]; // R
                    dest[dest_offset + 1] = src[src_offset + 1]; // G
                    dest[dest_offset + 2] = src[src_offset + 2]; // B
                    dest[dest_offset + 3] = src[src_offset + 3]; // A
                }
            }
        }

        // Unmap the staging texture
        rc.device_context()->Unmap(staging_texture.get(), 0);

        return img;
    }
} // namespace mge::dx11