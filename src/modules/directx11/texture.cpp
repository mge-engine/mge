// mge - Modern Game Engine
// Copyright (c) 2017-2026 by Alexander Schroeder
// All rights reserved.
#include "texture.hpp"
#include "error.hpp"
#include "mge/core/stdexceptions.hpp"
#include "mge/core/zero_memory.hpp"
#include "render_context.hpp"

namespace mge::dx11 {

    texture::texture(render_context& context, mge::texture_type type)
        : mge::texture(context, type)
    {}

    texture::texture(render_context&          context,
                     mge::texture_type        type,
                     const mge::image_format& format,
                     const mge::extent&       extent)
        : mge::texture(context, type, mge::texture_usage::RENDER_TARGET)
    {
        auto* device = dx11_context(context).device();

        D3D11_TEXTURE2D_DESC desc;
        mge::zero_memory(desc);
        desc.Width = extent.width;
        desc.Height = extent.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        bool is_depth =
            format.format() == mge::image_format::data_format::DEPTH ||
            format.format() == mge::image_format::data_format::DEPTH_STENCIL;

        if (is_depth) {
            DXGI_FORMAT typeless_fmt;
            DXGI_FORMAT dsv_fmt;
            DXGI_FORMAT srv_fmt;

            if (format.format() ==
                mge::image_format::data_format::DEPTH_STENCIL) {
                typeless_fmt = DXGI_FORMAT_R24G8_TYPELESS;
                dsv_fmt = DXGI_FORMAT_D24_UNORM_S8_UINT;
                srv_fmt = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            } else if (format.type() == mge::data_type::FLOAT) {
                typeless_fmt = DXGI_FORMAT_R32_TYPELESS;
                dsv_fmt = DXGI_FORMAT_D32_FLOAT;
                srv_fmt = DXGI_FORMAT_R32_FLOAT;
            } else if (format.type() == mge::data_type::UINT16) {
                typeless_fmt = DXGI_FORMAT_R16_TYPELESS;
                dsv_fmt = DXGI_FORMAT_D16_UNORM;
                srv_fmt = DXGI_FORMAT_R16_UNORM;
            } else {
                MGE_THROW(mge::illegal_argument)
                    << "Unsupported depth image format (data type): " << format;
            }

            desc.Format = typeless_fmt;
            desc.BindFlags =
                D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

            ID3D11Texture2D* tex = nullptr;
            auto hr = device->CreateTexture2D(&desc, nullptr, &tex);
            CHECK_HRESULT(hr, ID3D11Device, CreateTexture2D);
            m_texture.reset(tex);

            D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
            mge::zero_memory(dsv_desc);
            dsv_desc.Format = dsv_fmt;
            dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            dsv_desc.Texture2D.MipSlice = 0;

            ID3D11DepthStencilView* dsv = nullptr;
            hr = device->CreateDepthStencilView(m_texture.get(),
                                                &dsv_desc,
                                                &dsv);
            CHECK_HRESULT(hr, ID3D11Device, CreateDepthStencilView);
            m_depth_stencil_view.reset(dsv);

            D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
            mge::zero_memory(srv_desc);
            srv_desc.Format = srv_fmt;
            srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MostDetailedMip = 0;
            srv_desc.Texture2D.MipLevels = 1;

            ID3D11ShaderResourceView* srv = nullptr;
            hr = device->CreateShaderResourceView(m_texture.get(),
                                                  &srv_desc,
                                                  &srv);
            CHECK_HRESULT(hr, ID3D11Device, CreateShaderResourceView);
            m_shader_resource_view.reset(srv);

            D3D11_SAMPLER_DESC sampler_desc;
            mge::zero_memory(sampler_desc);
            sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            sampler_desc.MinLOD = 0;
            sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

            ID3D11SamplerState* sampler = nullptr;
            hr = device->CreateSamplerState(&sampler_desc, &sampler);
            CHECK_HRESULT(hr, ID3D11Device, CreateSamplerState);
            m_sampler_state.reset(sampler);
        } else {
            desc.Format = texture_format(format);
            desc.BindFlags =
                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

            ID3D11Texture2D* tex = nullptr;
            auto hr = device->CreateTexture2D(&desc, nullptr, &tex);
            CHECK_HRESULT(hr, ID3D11Device, CreateTexture2D);
            m_texture.reset(tex);

            D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
            mge::zero_memory(rtv_desc);
            rtv_desc.Format = desc.Format;
            rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtv_desc.Texture2D.MipSlice = 0;

            ID3D11RenderTargetView* rtv = nullptr;
            hr = device->CreateRenderTargetView(m_texture.get(),
                                                &rtv_desc,
                                                &rtv);
            CHECK_HRESULT(hr, ID3D11Device, CreateRenderTargetView);
            m_render_target_view.reset(rtv);

            D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
            mge::zero_memory(srv_desc);
            srv_desc.Format = desc.Format;
            srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srv_desc.Texture2D.MostDetailedMip = 0;
            srv_desc.Texture2D.MipLevels = 1;

            ID3D11ShaderResourceView* srv = nullptr;
            hr = device->CreateShaderResourceView(m_texture.get(),
                                                  &srv_desc,
                                                  &srv);
            CHECK_HRESULT(hr, ID3D11Device, CreateShaderResourceView);
            m_shader_resource_view.reset(srv);

            D3D11_SAMPLER_DESC sampler_desc;
            mge::zero_memory(sampler_desc);
            sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
            sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
            sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            sampler_desc.MinLOD = 0;
            sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

            ID3D11SamplerState* sampler = nullptr;
            hr = device->CreateSamplerState(&sampler_desc, &sampler);
            CHECK_HRESULT(hr, ID3D11Device, CreateSamplerState);
            m_sampler_state.reset(sampler);
        }
    }

    texture::~texture() {}

    DXGI_FORMAT texture::texture_format(const mge::image_format& format) const
    {
        switch (format.format()) {
        case mge::image_format::data_format::RGB:
            switch (format.type()) {
            case mge::data_type::FLOAT:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        case mge::image_format::data_format::RGBA:
            switch (format.type()) {
            case mge::data_type::UINT8:
                return DXGI_FORMAT_R8G8B8A8_UNORM;
            case mge::data_type::UINT16:
                return DXGI_FORMAT_R16G16B16A16_UNORM;
            case mge::data_type::FLOAT:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default:
                MGE_THROW(illegal_argument)
                    << "Unsupported image format (data type): " << format;
            }
        default:
            MGE_THROW(illegal_argument)
                << "Unsupported image format (format): " << format;
        }
    }

    void texture::set_data(const mge::image_format& format,
                           const mge::extent&       extent,
                           const void*              data,
                           size_t                   size)
    {
        D3D11_TEXTURE2D_DESC desc;
        mge::zero_memory(desc);
        desc.Width = extent.width;
        desc.Height = extent.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = texture_format(format);
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresource_data;
        mge::zero_memory(subresource_data);
        subresource_data.pSysMem = data;
        subresource_data.SysMemPitch =
            static_cast<UINT>(extent.width * format.binary_size());
        subresource_data.SysMemSlicePitch = 0;

        ID3D11Texture2D* texture = nullptr;

        auto* device = dx11_context(context()).device();

        auto hr = device->CreateTexture2D(&desc, &subresource_data, &texture);
        CHECK_HRESULT(hr, ID3D11Device, CreateTexture2D);
        m_texture.reset(texture);

        D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
        mge::zero_memory(srv_desc);
        srv_desc.Format = desc.Format;
        srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MostDetailedMip = 0;
        srv_desc.Texture2D.MipLevels = 1;

        ID3D11ShaderResourceView* srv = nullptr;
        hr = device->CreateShaderResourceView(m_texture.get(), &srv_desc, &srv);
        CHECK_HRESULT(hr, ID3D11Device, CreateShaderResourceView);
        m_shader_resource_view.reset(srv);

        D3D11_SAMPLER_DESC sampler_desc;
        mge::zero_memory(sampler_desc);
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampler_desc.MinLOD = 0;
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

        ID3D11SamplerState* sampler = nullptr;
        hr = device->CreateSamplerState(&sampler_desc, &sampler);
        CHECK_HRESULT(hr, ID3D11Device, CreateSamplerState);
        m_sampler_state.reset(sampler);
    }
} // namespace mge::dx11